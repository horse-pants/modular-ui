var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onLoad);

function onLoad(event) {
    initWebSocket();
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
    websocket.send("states");
}
  
function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
} 

function onMessage(event) {
    var myObj = JSON.parse(event.data);
            console.log(myObj);
            for (i in myObj.controls){
                var name = myObj.controls[i].name;
                var state = myObj.controls[i].state;
                console.log(name);
                console.log(state);
                if (state == "1"){
                    document.getElementById(name).checked = true;
                    document.getElementById(name+"s").innerHTML = "ON";
                }
                else if (state == "0"){
                    document.getElementById(name).checked = false;
                    document.getElementById(name+"s").innerHTML = "OFF";
                }
                else if (name=="color"){
                    document.getElementById(name).value = state;
                }
            }
    console.log(event.data);
}

// Send Requests to Control GPIOs
function toggleControl (element) {
    console.log(element.id);
    websocket.send(element.id);
    if (element.checked){
        document.getElementById(element.id+"s").innerHTML = "ON";
    }
    else {
        document.getElementById(element.id+"s").innerHTML = "OFF"; 
    }
}

function colorChange (element) {
    websocket.send(element.value);
}

function animationChange (element) {
    websocket.send(element.value);
}
