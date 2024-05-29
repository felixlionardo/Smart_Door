import {Server} from 'socket.io';
import dgram from 'dgram';
import child from 'child_process';
var io;


function beagleboneServer(server) {
    io = new Server(server);
    io.sockets.on('connection', function(socket) {
        handleCommand(socket);
    });
}

function handleCommand(socket) {
	// Pased string of comamnd to relay
	socket.on('message', function(data) {
		console.log('message command: ' + data);

		// Info for connecting to the local process via UDP
		var PORT = 12345;
		var HOST = '192.168.7.2';
		var buffer = new Buffer(data);

		var client = dgram.createSocket('udp4');
		client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
			if (err) 
				throw err;
			console.log('UDP message sent to ' + HOST +':'+ PORT);
		});

		client.on('listening', function () {
			var address = client.address();
			console.log('UDP Client: listening on ' + address.address + ":" + address.port);
		});
		// Handle an incoming message over the UDP from the local application.
		client.on('message', function (message, remote) {
			console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);

			var reply = message.toString('utf8')
			socket.emit('commandReply', reply);

			client.close();

		});
		client.on("UDP Client: close", function() {
			console.log("closed");
		});
		client.on("UDP Client: error", function(err) {
			console.log("error: ",err);
		});
		
		let ffmpeg = child.spawn("ffmpeg", [
			"-re",
			"-y",
			"-i",
			"udp://192.168.7.1:12345",
			"-preset",
			"ultrafast",
			"-f",
			"mjpeg",
			"pipe:1"
		]);
		
		ffmpeg.on('error', function (err) {
			console.log(err);
			throw err;
		});
		
		ffmpeg.on('close', function (code) {
			console.log('ffmpeg exited with code ' + code);
		});
		
		ffmpeg.stderr.on('data', function(data) {
			// Don't remove this
			// Child Process hangs when stderr exceed certain memory
		});
		
		ffmpeg.stdout.on('data', function (data) {
			var frame = Buffer.from(data).toString('base64'); //convert raw data to string
			io.sockets.emit('canvas',frame); //send data to client
		});
	});
};

export { beagleboneServer };
