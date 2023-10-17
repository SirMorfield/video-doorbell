// This script reads the occupants.conf in the ./app directory, and generates all the different config files that are needed for the indoor panels and asterisk
import nodeFs from 'fs'
import { execSync } from 'child_process'

function relative(path: string): string {
	path = path.trim()
	if (path.startsWith('/')) {
		return path
	}
	return `${__dirname}/${path}`
}

const fs = {
	readFileSync(path: string): string { return nodeFs.readFileSync(relative(path)).toString() },
	writeFileSync(path: string, content: string) { nodeFs.writeFileSync(relative(path), content) },
	mkdirSync(path: string) { return nodeFs.mkdirSync(relative(path), { recursive: true }) },
	existsSync(path: string) { return nodeFs.existsSync(relative(path)) },
	copyFileSync(from: string, to: string) { return nodeFs.copyFileSync(relative(from), relative(to)) },
} as const

function exec(command: string) {
	console.log('Executing:', command)
	return execSync(command).toString()
}

function generateExtension(number: string, dial: string): string {
	return fs.readFileSync('extensions_client.template.conf').replaceAll('<PHONE_NUMBER>', number).replaceAll('<DIAL>', dial)
}

function generateSIPclient(number: string): string {
	return fs.readFileSync('sip_client.template.conf').replaceAll('<PHONE_NUMBER>', number)
}

interface Occupant {
	name: string;
	numbers: string[];
}

const occupants: Occupant[] = fs.readFileSync('../app/occupants.conf')
	.toString()
	.replaceAll('\r', '')
	.split('\n')
	.filter(line => line.split(',').length >= 2 && !/\s*#/g.test(line))
	.map(line => {
		return {
			name: line.split(',')[0]!.trim(),
			numbers: line.split(',').slice(1).map(number => number.trim())
		}
	})

const outDir = 'generated'
fs.mkdirSync(outDir)

const templates = {
	extensions: fs.readFileSync('extensions.template.conf'),
	sip: fs.readFileSync('sip.template.conf'),
	indoorStation: fs.readFileSync('./config_i53W.template.txt'),
}

const numbersDone: string[] = []
for (const occupant of occupants) {

	for (const number of occupant.numbers) {
		if (!numbersDone.includes(number)) {
			let config = templates.indoorStation.replaceAll('<PHONE_NUMBER>', occupant.numbers[0]!)
			fs.writeFileSync(`${outDir}/config_i53W_${occupant.numbers[0]}.txt`, config)
			templates.sip += generateSIPclient(number)
			templates.extensions += generateExtension(number, `SIP/${number}`)
			numbersDone.push(number)
		}
	}
	if (occupant.numbers.length > 1) {
		templates.extensions += generateExtension(occupant.numbers.join(''), occupant.numbers.map(n => `SIP/${n}`).join('&'))
	}
}

const extensionsPath = '../asterisk/extensions.conf'
fs.writeFileSync(extensionsPath, templates.extensions)
console.log('Wrote extentions to ' + extensionsPath)

const sipPath = '../asterisk/sip.conf'
fs.writeFileSync(sipPath, templates.sip)
console.log('Wrote SIP conf to ' + sipPath)

console.log(`Found ${occupants.length} clients`)
console.log(`Generated ${numbersDone.length} indoor station configs in ./${outDir}`)

const asterisk = '/etc/asterisk'
if (!fs.existsSync(asterisk)) {
	console.log('Asterisk not found at, skipping asterisk reload')
}
else {
	fs.copyFileSync('../asterisk/extensions.conf', `${asterisk}/extensions.conf`)
	fs.copyFileSync('../asterisk/sip.conf', `${asterisk}/sip.conf`)
	exec('asterisk -rx "reload"')
}
