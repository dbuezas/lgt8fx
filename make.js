#!/usr/bin/env node
const fs = require("fs"); //Load the filesystem module
const { execSync } = require("child_process");

const package = "package_lgt8fx_index.json";
const folder = "lgt8f";

const json = JSON.parse(fs.readFileSync(package));
const { platforms } = json.packages[0];
const [major, minor, patch] = platforms[0].version.split(".");

const newVersion = [major, minor, parseFloat(patch) + 1].join(".");
const archiveFileName = `${folder}-${newVersion}.zip`;
execSync(`zip -r ${archiveFileName} ${folder}`);
console.warn(`zipped ${archiveFileName}`);
const size = fs.statSync(archiveFileName).size.toString();
console.warn(`size ${size}`);
const checksum =
  "SHA-256:" +
  execSync(`shasum -a 256 ${archiveFileName}`).toString().split(" ")[0];
console.warn(`checksum ${checksum}`);

platforms.unshift({
  name: "LGT8fx",
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
    { name: "LGT8F328P-LQFP32 MiniEVB" },
    { name: "LGT8F328P-LQFP48 MiniEVB" },
    { name: "LGT8F328P-LQFP32 wemos-TTGO-XI" },
    { name: "LGT8F328P-SSOP20" },
    { name: "LGT8F328D-LQFP32" },
  ],
});

fs.writeFileSync(package, JSON.stringify(json, 0, 2));
console.log(newVersion);
