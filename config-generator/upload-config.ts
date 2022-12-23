// This script does not work
import fs from 'fs'

async function uploadConfig(ip: string, config: string): Promise<void> {
	let text1 = await (await fetch(`http://${ip}/`, {
		"credentials": "include",
		"headers": {
			"User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:108.0) Gecko/20100101 Firefox/108.0",
			"Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8",
			"Accept-Language": "en-US,en;q=0.5",
			"Content-Type": "application/x-www-form-urlencoded",
			"Upgrade-Insecure-Requests": "1",
			"cookie": `CUR_LANGCLICK=false; CUR_LANG=false`
		},
		"referrer": `http://${ip}/`,
		"body": "encoded=root%3A5d2a444347a912e3009e8fad048e0409&CurLanguage=en&ReturnPage=%2F",
		"method": "POST",
		"mode": "cors"
	})).text()


	const auth = await (await fetch(`http://${ip}/key==nonce`, {
		"credentials": "include",
		"headers": {
			"User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:108.0) Gecko/20100101 Firefox/108.0",
			"Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8",
			"Accept-Language": "en-US,en;q=0.5",
			"Upgrade-Insecure-Requests": "1",
			"cookie": `CUR_LANGCLICK=false; CUR_LANG=false`
		},
		"method": "GET",
		"mode": "cors"
	})).text()

	console.log('auth', auth)


	let text
	try {
		const resp = await fetch(`http://${ip}/config.htm`, {
			credentials: "include",
			headers: {
				"User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:108.0) Gecko/20100101 Firefox/108.0",
				"Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8",
				"Accept-Language": "en-US,en;q=0.5",
				"Content-Type": "multipart/form-data; boundary=---------------------------380989151839902410932090124059",
				// "Upgrade-Insecure-Requests": "1",
				"Pragma": "no-cache",
				"Cache-Control": "no-cache",
				"cookie": `auth=${auth}; CUR_LANGCLICK=false; CUR_LANG=false`
			},
			referrer: `http://${ip}/config.htm`,
			body: `-----------------------------380989151839902410932090124059\r\nContent-Disposition: form-data; name=\"CONFIG\"; filename=\"config_i53W_004.txt\"\r\nContent-Type: text/plain\r\n\r\n${config}\r\n-----------------------------380989151839902410932090124059--\r\n`,
			method: "POST",
			mode: "cors",
		})

		text = await resp?.text()
	} catch (e) {
		console.log('e', e)
	}

	if (!text.includes('<title>Login</title>')) {
		console.log('success', ip, auth)
		return

	}
	console.log('failed', ip)
}
// 6802a8c0000030a8

void async function () {
	const units = [
		{
			ip: '192.168.2.120',
			unit: '001',
		},
		{
			ip: '192.168.2.119',
			unit: '002',
		},
		{
			ip: '192.168.2.127',
			unit: '003',
		},
		{
			ip: '192.168.2.122',
			unit: '004',
		},
		{
			ip: '192.168.2.126',
			unit: '006',
		},
		{
			ip: '192.168.2.132',
			unit: '007',
		},
		{
			ip: '192.168.2.128',
			unit: '009',
		},
		{
			ip: '192.168.2.123',
			unit: '010',
		},
		{
			ip: '192.168.2.133',
			unit: '011',
		},
		{
			ip: '192.168.2.125',
			unit: '101',
		},
		{
			ip: '192.168.2.134',
			unit: '1031',
		},
		{
			ip: '192.168.2.124',
			unit: '1032',
		},
		{
			ip: '192.168.2.130',
			unit: '104',
		},
		{
			ip: '192.168.2.129',
			unit: '105',
		},
		{
			ip: '192.168.2.135',
			unit: '201',
		},
		{
			ip: '192.168.2.139',
			unit: '202',
		},
		{
			ip: '192.168.2.136',
			unit: '301',
		},
		{
			ip: '192.168.2.137',
			unit: '302',
		},
		{
			ip: '192.168.2.138',
			unit: '303',
		},
		{
			ip: '192.168.2.131',
			unit: '304',
		},
	]

	// 6802a8c0000030a8
	// http://192.168.2.138/key==nonce
	for (const unit of units) {
		console.log(unit.ip, unit.unit)
		const config = (await fs.promises.readFile(`./generated/config_i53W_${unit.unit}.txt`)).toString()

		await uploadConfig(unit.ip, config)
	}
}()
