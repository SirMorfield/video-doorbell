import fs from 'fs'

// 6802a8c000000c04
// 6802a8c000000012
// 6802a8c00000004e
// 6802a8c0000000b0

async function uploadConfig(ip: string, config: string): Promise<void> {
	// return await (await fetch('https://find-peers.codam.nl/status/pull')).json()

	for (let i = 0; i <= 255 * 255; i++) {
		const auth = `6802a8c00000${i.toString(16).padStart(4, '0')}`
		console.log('Trying', auth, i)
		// await new Promise(r => setTimeout(r, 500))
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
			}).catch(e => undefined)

			text = await resp?.text()
		} catch (e) {
			continue
		}
		if (!text.includes('<title>Login</title>')) {
			console.log('success', auth, i)
			break
		}
	}
}

void async function () {
	const units = [
		// 	{
		// 	ip: '192.168.2.120',
		// 	unit: '001',
		// }, {
		// 	ip: '192.168.2.119',
		// 	unit: '001',
		// }, {
		// 	ip: '192.168.2.127',
		// 	unit: '003',
		// },
		// {
		// 	ip: '192.168.2.122',
		// 	unit: '004',
		// },
		// {
		// 	ip: '192.168.2.126',
		// 	unit: '006',
		// },
		{
			ip: '192.168.2.132',
			unit: '007',
		},
	]

	for (const unit of units) {
		const config = (await fs.promises.readFile(`./generated/config_i53W_${unit.unit}.txt`)).toString()

		await uploadConfig(unit.ip, config)
		return
	}
}()
