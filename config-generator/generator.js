// This script reads the occupants.csv in the ./app directory, and generates all the different config files that are needed for the indoor panels and asterisk
// Requires NodeJS >= 17

const fs = require('fs')

function generateExtension(number) {
	return fs.readFileSync('extensions_client.template.conf').toString().replaceAll('<PHONE_NUMBER>', number)
}

function generateSIPclient(number) {
	return fs.readFileSync('sip_client.template.conf').toString().replaceAll('<PHONE_NUMBER>', number)
}

const occupantNumbers = fs.readFileSync('../app/occupants.csv')
	.toString()
	.split('\n')
	.filter(line => line.includes(','))
	.map(line => line.split(',')[0])

const outDir = 'generated'
fs.mkdirSync(outDir, { recursive: true })
const template = fs.readFileSync('./config_i53W.template.txt').toString()

let extensionsConf = fs.readFileSync('extensions.template.conf').toString()
let sipConf = fs.readFileSync('sip.template.conf').toString()

const unique = uniq = [...new Set(occupantNumbers)].sort()
for (const number of unique) {
	let config = template.replaceAll('<PHONE_NUMBER>', number)
	fs.writeFileSync(`${outDir}/config_i53W_${number}.txt`, config)

	extensionsConf += generateExtension(number)
	sipConf += generateSIPclient(number)
}

const extensionsPath = '../asterisk/extensions.conf'
fs.writeFileSync(extensionsPath, extensionsConf)
console.log('Wrote extentions to ' + extensionsPath)

const sipPath = '../asterisk/sip.conf'
fs.writeFileSync(sipPath, sipConf)
console.log('Wrote SIP conf to ' + sipPath)

console.log(`Generated ${unique.length} clients`)
