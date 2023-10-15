import { Page } from "puppeteer";
import { openBrowser } from "./browser";
import { execSync } from "child_process"

const commands = {
	'set-time': 'Sets the time of this device, usage example: config settime 2021-01-01 12:00',
	'get-time': 'Gets the time of this device',
	'sync-indoor-units': 'Syncs the time on all i53w devices',
	'show-peers': 'Shows all indoor units and cameras that are connected',
	'update-occupants': 'After changing the occupants.conf, run this command to update the app to use the new occupants'
} as const

const command = process.argv[2]?.toLowerCase() as keyof typeof commands
if (!commands[command]) {
	console.log('No command found, usage: config [command] [args]')
	console.log('Example: config settime 2021-01-01 12:00')
	console.log('Available commands:')
	const longest = Object.keys(commands).reduce((a, b) => a.length > b.length ? a : b).length
	for (const [name, desc] of Object.entries(commands)) {
		console.log(`  ${name.padEnd(longest)}: ${desc}`)
	}
	exitWith(1)
}


if (command === 'set-time') {
	const args = process.argv.slice(3).join(" ")
	console.log('Reading date from input:', args)
	const date = new Date(args)
	if (date.toString() === 'Invalid Date') {
		exitWith(1, 'Invalid date: ' + args + '\nUsage example: config settime 2021-01-01 12:00')
	}
	setLocalTime(date)
	exitWith(0)
}
if (command === 'get-time') {
	console.log(formatDate(new Date()))
	exitWith(0)
}

if (command === 'sync-indoor-units') {
	const units = getI53W()
	console.log('Found peers:')
	console.log(units.map(u => `${u.name} ${u.ip}`).join('\n'))
	await syncI53Ws(units)
	exitWith(0)
}

if (command === 'show-peers') {
	console.log(showPeers())
	exitWith(0)
}

if (command === 'update-occupants') {
	exec('/root/video-doorbell/production.sh', true)
	exitWith(0)
}

// Utils below

function exec(command: string, inheritIO = false) {
	console.log('Executing:', command)
	const opt = inheritIO ? { stdio: 'inherit' } as const : {}
	return execSync(command, opt).toString()
}

async function exitWith(code: 1 | 0, msg = '') {
	if (msg) {
		console.log(msg)
	}
	process.exit(code)
}

function destructDate(d: Date) {
	return {
		d: `${d.getFullYear()}-${(d.getMonth() + 1)}-${d.getDate()}`,
		h: d.getHours().toString(),
		m: d.getMinutes().toString(),
	}
}

function formatDate(d: Date) {
	const { d: date, h, m } = destructDate(d)
	return `${date} ${h.padStart(2, '0')}:${m.padStart(2, '0')}`
}

async function syncI53Ws(ips: { name: string, ip: string }[]) {
	const { page, browser } = await openBrowser(true)
	for (const { ip , name} of ips) {
		try {
			const d = new Date()
			d.setHours(Math.round(Math.random() * 24))
			console.log(`Setting time for ${name}\t${ip}\t${formatDate(d)}`)
			await setDateTimeI53W(page, `http://${ip}`, d)
		} catch (e) {
			console.error(e)
		}
	}
	await browser.close();
}

async function setDateTimeI53W(page: Page, link: string, date: Date) {
	await page.goto(link)
	try {
		await page.type("#username", "root")
		await page.type("#password", "joppe2022")
		await page.click("#logonButton")
		await page.waitForNavigation()
	} catch (e) { }
	await page.goto(link + "/time.htm")

	const { d, h, m } = destructDate(date)
	console.log(`Setting date to  ${formatDate(date)}`)
	await page.evaluate((d) => {
		(document.querySelector("#calendarSet") as HTMLInputElement).value = d
	}, d)

	await page.select("#TIM_ManualHour", h)
	await page.select("#TIM_ManualMinute", m)
	await page.click("body > div.content > div.navbar > form:nth-child(2) > div.OptDiv > table > tbody > tr > td:nth-child(5) > input")
}

function setLocalTime(date: Date) {
	console.log(`Setting date to ${formatDate(date)}}`)
	const { d, h, m } = destructDate(date)
	exec(`date -s "${d} ${h}:${m}"`)
	exec(`hwclock -w`);
	console.log(`Date set to ${formatDate(new Date())}`)
}

function showPeers() {
	return exec("asterisk -rx 'sip show peers'")
}

function getI53W() {
	return showPeers()
		.split("\n")
		.map(line => line.trim())
		.map(line => line.match(/^(\d+\/\d+)\s+(\d+\.\d+\.\d+\.\d+)/)) // 001/001 192.168.2.101 D Yes Yes 5108  Unmonitored
		.filter(Boolean).map(line => line as string[])
		.map(([_, name, ip]) => ({ name, ip }))
}
