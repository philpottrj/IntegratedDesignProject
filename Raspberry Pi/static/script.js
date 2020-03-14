$(document).ready(setInterval(updateValues(), 1000))

function updateValues() {
    var nl = false;
    var ac = false;
    var ss = false;
    var temp = 70;
    var intruder = false;
    document.getElementById('nightLight').innerHTML = "Night Light: " + nl;
    document.getElementById('airConditioning').innerHTML = "Air Conditioning: " + ac;
    document.getElementById('smartSwitch').innerHTML = "Smart Switch: " + ss;
    document.getElementById('temperature').innerHTML = "Temperature: " + temp;
    document.getElementById('intruderAlert').innerHTML = "Intruder: " + intruder;
}