setInterval(
function updateValues() {
    document.getElementById('nightLight').innerHTML = "Night Light: " + nl;
    document.getElementById('airConditioning').innerHTML = "Air Conditioning: " + ac;
    document.getElementById('smartSwitch').innerHTML = "Smart Switch: " + ss;
    document.getElementById('temperature').innerHTML = "Temperature: " + temp;
    document.getElementById('intruderAlert').innerHTML = "Intruder: " + intruder;
}, 1000)