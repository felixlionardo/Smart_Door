var socket = io.connect();
var replyTimeoutDuration;
var communicationTimeout;
var inputCount = 0;

function showErrorBox() {
    replyTimeoutDuration = setTimeout(function() {
        $('#error-box').show();
    }, 100);
}

function resetCommunicationTimeout() {
    clearTimeout(communicationTimeout);
    communicationTimeout = setTimeout(() => {
        clearInputs();
    }, 1000);
}


function appendInput(event) {
    var input = document.getElementById("userInput").value;

    if (input !== '' && inputCount < 5) {
        var currentContent = document.getElementById("displayArea").innerHTML;
        document.getElementById("displayArea").innerHTML = currentContent + input + '<br>';

        socket.emit('message', input);

        document.getElementById("userInput").value = '';
        inputCount++;
    }
}

function clearInputs() {
    document.getElementById("displayArea").innerHTML = '';
    inputCount = 0;

    socket.emit('message', 'clear');
}
  

$(document).ready(function() {
    clearInputs();
    
    $('#lock').click(function() {
        socket.emit('message', 'lock');
    });
    $('#unlock').click(function() {
        socket.emit('message', 'unlock');
    });
    $('#celcius').click(function() {
        socket.emit('message', 'celcius');
        $('#temp_unit').text("C");
    });
    $('#fahrenheit').click(function() {
        socket.emit('message', 'fahrenheit');
        $('#temp_unit').text("F");
    });

    setInterval(function() {
        socket.emit('message', 'info');
        showErrorBox();
    }, 100);


    socket.on('commandReply', function(command) {
        console.log('Received command: ' + command);
        $('#error-box').hide();
        clearTimeout(replyTimeoutDuration);
        resetCommunicationTimeout();

        var temperature = command.split("\n")[0].split(":")[1];
        var lockstatus = command.split("\n")[1].split(":")[1];
        
        $('#temp_value').text(temperature);
        $('#modeid').text(lockstatus);
    });
    socket.on('canvas', function(data) {
        console.log('Received canvas data');
        const canvas = $("#videostream");  
        const ctx = canvas[0].getContext('2d');
        const img = new Image();
        img.src = 'data:image/jpeg;base64,' + data;
        img.onload = function() {
            ctx.height = img.height;
            ctx.width = img.width;
            ctx.drawImage(img, 0, 0, ctx.width, ctx.height);
        }
    });
});


