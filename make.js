#!/usr/bin/env node
const fs = require("fs"); //Load the filesystem module
const { execSync } = require("child_process");

const package = "package_lgt8fx_index.json";
const folder = "lgt8f";

const toNum = (str) => parseInt(str.split(".").join(""), 10);
const jsonStr = fs.readFileSync(package);

const json = JSON.parse(jsonStr);
const { platforms } = json.packages[0];
let lastVersion = "0.9.9";
for (const { version } of platforms) {
  if (toNum(version) > toNum(lastVersion)) {
    lastVersion = version;
  }
}

const newVersion = (toNum(lastVersion) + 1 + "").split("").join(".");

const archiveFileName = `${folder}-${newVersion}.zip`;
execSync(`zip -r ${archiveFileName} ${folder}`);
console.log(`zipped ${archiveFileName}`);
const size = fs.statSync(archiveFileName).size.toString();
console.log(`size ${size}`);
const checksum =
  "SHA-256:" +
  execSync(`shasum -a 256 ${archiveFileName}`).toString().split(" ")[0];
console.log(`checksum ${checksum}`);

platforms.unshift({
  name: "LGT8fx Boards",
  architecture: "avr",
  version: newVersion,
  category: "lgt8fx",
  url:
    "https://github.com/dbuezas/lgt8fx/releases/download/v" +
    newVersion +
    "/" +
    archiveFileName,
  archiveFileName,
  checksum,
  size,
  help: {
    online: "https://github.com/dbuezas/LGT8fx/issues",
  },
  boards: [
    { name: "LGT8F328P-LQFP48 MiniEVB" },
    { name: "LGT8F328P-LQFP32 wemos-TTGO-XI" },
    { name: "LGT8F328P-LQFP32 MiniEVB" },
    { name: "LGT8F328P-SSOP20" },
    { name: "LGT8F328D-LQFP32" },
    { name: "LGT8F328D-SSOP20" },
    { name: "LGT8F88D-SSOP20" },
  ],
});

fs.writeFileSync(package, JSON.stringify(json, 0, 2));
