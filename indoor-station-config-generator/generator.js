// This script reads the occupants.csv in the ./app directory, and generates all the different config files that are  for the
// Requires NodeJS >= 17

const fs = require('fs')

const occupantNumbers = fs.readFileSync('../app/occupants.csv')
	.toString()
	.split('\n')
	.filter(line => line.includes(','))
	.map(line => line.split(',')[0])

const outDir = 'generated'
fs.mkdirSync(outDir, { recursive: true })
const template = fs.readFileSync('./config_i53W.template.txt').toString()

const unique = uniq = [...new Set(occupantNumbers)]
for (const number of unique) {
	let config = template.replaceAll('<>', number)
	fs.writeFileSync(`${outDir}/config_i53W_${number}.txt`, config)
}
