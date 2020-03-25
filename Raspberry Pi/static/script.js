// Function that updates values in HTML
function updateValues() {
    // Create an AJAX request
    $.ajax({
        type: "GET",  // GET request (as opposed to "POST")
        url: "/data", // Go to /data URL
        complete: function(data) { // When AJAX request successfully completes
            var array = String(data.responseText).split(', '); // Returns /data page and converts to array
            // Assign array to variables
            var nl = Boolean(Number(array[0]));
            var ac = Boolean(Number(array[1]));
            var ss = Boolean(Number(array[2]));
            var temp = Number(array[3]);
            var intruder = Boolean(Number(array[4]));
            // Insert variables into HTML
            document.getElementById('nightLight').innerHTML = nl;
            document.getElementById('airConditioning').innerHTML = ac;
            document.getElementById('smartSwitch').innerHTML = ss;
            document.getElementById('temperature').innerHTML = temp + "F";
            document.getElementById('intruderAlert').innerHTML = intruder;
        }
    });
}

// Execute updateValues() on load of webpage
$(document).ready(updateValues())
// Execute updateValues() every 300 milliseconds after webpage is loaded
setInterval(updateValues, 300);