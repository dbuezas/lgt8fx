const fs = require("fs");

const arduinoFile = "./BruteForceTimings.ino";

const SerialPort = require("serialport");
let port;

const attach = () => {
  let [, baudRate] = fs
    .readFileSync(arduinoFile)
    .toString()
    .match(/.*#define BAUDS (\d+).*/);
  baudRate = parseInt(baudRate, 10);
  console.log(baudRate);
  if (port) port.close(() => {});
  port = new SerialPort(
    "/dev/cu.usbserial-A50285BI",
    {
      baudRate
    },
    e => {
      if (e) {
        console.error(e);
        setTimeout(attach, 100);
      }
    }
  );
  const sleep = ms => new Promise(r => setTimeout(r, ms));
  let packet = "";
  let timeout = -1;
  port.on("data", async data => {
    packet += data.toString();
    clearInterval(timeout);
    timeout = setTimeout(() => {
      process.stdout.write(packet);
      port.write(packet);
      packet = "";
    }, 100);
  });
};

fs.watchFile("./softwareSerial.ino", attach);
attach();
