import { PUPPETEER_REVISIONS } from "puppeteer-core/internal/revisions.js";
import * as browsers from "@puppeteer/browsers";
import puppeteer, { Product } from "puppeteer";
import os from "os";


// ensure the browser is installed.
// NB this is required because Bun does not execute arbitrary dependencies
//    lifecycle scripts, such as postinstall. even if it did, currently,
//    puppeteer assumes node is being used, so that would not work either.
//    see https://github.com/puppeteer/puppeteer/blob/puppeteer-v21.1.1/packages/puppeteer/package.json#L43
//    see https://bun.sh/docs/cli/install#trusted-dependencies
async function browserInstall() {
	let downloaded = false;
	const chromeVersion = PUPPETEER_REVISIONS.chrome;
	return await browsers.install({
		browser: "chrome",
		buildId: chromeVersion,
		cacheDir: `${os.homedir()}/.cache/puppeteer`,
		downloadProgressCallback: (downloadedBytes, totalBytes) => {
			if (!downloaded) {
				downloaded = true;
				console.log(`Downloading the browser Chrome/${chromeVersion}...`);
			}
		},
	});
}


export async function openBrowser(headless: boolean) {
	const installedBrowser = await browserInstall();
	const browser = await puppeteer.launch({
		product: installedBrowser.browser as Product,
		executablePath: installedBrowser.executablePath,
		args: [
			// "--start-maximized"
			"--no-sandbox", // required for root execution
			`--window-size=1920,1080`,

		],
		//headless: "new",
		headless: headless ? "new" : false,
		defaultViewport: {
			width: 1920,
			height: 1080
		}
	});
	const page = await browser.newPage();
	const client = await page.target().createCDPSession()
	await client.send('Network.clearBrowserCookies')
	await page.deleteCookie(...(await page.cookies()))
	return { browser, page };
}
