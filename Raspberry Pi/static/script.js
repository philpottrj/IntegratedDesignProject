function updateValues() {
    $.ajax({
        async: false,
        type: "GET",
        url: "/data",
        complete: function(data) { 
            var array = String(data.responseText).split(', ');
            var nl = Boolean(Number(array[0]));
            var ac = Boolean(Number(array[1]));
            var ss = Boolean(Number(array[2]));
            var temp = Number(array[3]);
            var intruder = Boolean(Number(array[4]));
            document.getElementById('nightLight').innerHTML = "Night Light: " + nl;
            document.getElementById('airConditioning').innerHTML = "Air Conditioning: " + ac;
            document.getElementById('smartSwitch').innerHTML = "Smart Switch: " + ss;
            document.getElementById('temperature').innerHTML = "Temperature: " + temp + "F";
            document.getElementById('intruderAlert').innerHTML = "Intruder: " + intruder;
        }
    });
}

$(document).ready(updateValues())
setInterval(updateValues, 300);