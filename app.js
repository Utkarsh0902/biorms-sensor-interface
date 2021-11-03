const SerialPort = require('serialport');
const axios = require('axios');

const serverURL = "http://localhost:3000/sensors";
const parsers = SerialPort.parsers;
const parser = new parsers.Readline({
    delimiter: '\r'
});

var port = new SerialPort('/dev/ttyACM0',{ 
    baudRate: 9600,
    dataBits: 8,
    parity: 'none',
    stopBits: 1,
    flowControl: false
});

port.pipe(parser);

parser.on('data', (data)=> {
    console.log("Data recieved");
    const JSONdata = JSON.parse(data);
    // use axios to send a POST request
    axios({
        method: 'POST',
        url: serverURL,
        data: JSONdata
    })
    .then(data=>console.log("Data sent"))
    .catch(err=>console.log(err));
    
});