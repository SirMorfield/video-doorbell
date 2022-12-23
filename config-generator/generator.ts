// This script reads the occupants.conf in the ./app directory, and generates all the different config files that are needed for the indoor panels and asterisk
// Requires NodeJS >= 17

import fs from 'fs'


function generateExtension(number: string, dial: string): string {
	return fs.readFileSync('extensions_client.template.conf').toString().replaceAll('<PHONE_NUMBER>', number).replaceAll('<DIAL>', dial)
}

function generateSIPclient(number: string): string {
	return fs.readFileSync('sip_client.template.conf').toString().replaceAll('<PHONE_NUMBER>', number)
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
fs.mkdirSync(outDir, { recursive: true })

const templates = {
	extensions: fs.readFileSync('extensions.template.conf').toString(),
	sip: fs.readFileSync('sip.template.conf').toString(),
	indoorStation: fs.readFileSync('./config_i53W.template.txt').toString(),
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
