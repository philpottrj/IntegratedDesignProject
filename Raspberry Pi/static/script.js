// Define Color Variables
var colors = {
    red: "#DC3545", // old red: "#C23127",
    yellow: "#FFC107", // old yellow: "#FFCC02",
    orange: "#FD7E14",
    green: "#28A745", // old green: "#4BD964",
    blue: "#007BFF",
    disabled: "#6C757D", // old disabled: "#CCCCCC",
    error: "#E83E8C" // old error: "#FF0080"
}

// Function that updates values in HTML
function updateValues() {
    // Create an AJAX request
    $.ajax({
        type: "GET",  // GET request (as opposed to "POST")
        url: "/data", // Go to /data URL
        complete: function(data) { // When AJAX request successfully completes
            var array = String(data.responseText).split(', '); // Returns /data page and converts to array
            // Assign array to variables
            var nl = boolToState(Boolean(Number(array[0])));
            var ac = boolToState(Boolean(Number(array[1])));
            var ss = boolToState(Boolean(Number(array[2])));
            var temp = Number(array[3]);
            var intruder = boolToState(Boolean(Number(array[4])));
            // Update all icons
            updateNL(nl);
            updateAC(ac);
            updateSS(ss);
            updateTemp(temp);
            updateIA(intruder);
        }
    });
}

$("input").click(function(e) {
    e.preventDefault();
    $.ajax({
        type: "POST",
        url: "/",
        data: $(this)
    });
});

function boolToState(bool) { 
    let state = "Off";
    if(bool) {
        state = "On";
    }
    return state;
}

function stateToBool(state) { 
    let bool = false;
    if(state == "On") {
        bool = true;
    }
    return bool;
}

function updateAC(state) {
    if (stateToBool(state)) {
        $('#airConditioning-icon').addClass('fa-spin');
        color = colors.blue;
    }
    else {
        $('#airConditioning-icon').removeClass('fa-spin');
        color = colors.disabled;
    }
    document.getElementById('airConditioning').innerHTML = state;
    document.getElementById('airConditioning-icon').style.color = color;
}

function updateSS(state) {
    if (stateToBool(state)) {
        $('#smartSwitch-icon').removeClass('fa-toggle-off').addClass('fa-toggle-on');
        color = colors.green;
    }
    else {
        $('#smartSwitch-icon').removeClass('fa-toggle-on').addClass('fa-toggle-off');
        color = colors.disabled;
    }
    document.getElementById('smartSwitch-icon').style.color = color;
    document.getElementById('smartSwitch').innerHTML = state;
}

function updateIA(state) {
    if (stateToBool(state)) {
        $('#intruderAlert-icon').addClass('blink');
        color = colors.red;
    }
    else {
        $('#intruderAlert-icon').removeClass('blink');
        color = colors.disabled;
    }
    document.getElementById('intruderAlert').innerHTML = state;
    document.getElementById('intruderAlert-icon').style.color = color;
}

function updateNL(state) {
    if (stateToBool(state)) {
        color = colors.yellow;
    }
    else {
        color = colors.disabled;
    }
    document.getElementById('nightLight').innerHTML = state;
    document.getElementById('nightLight-icon').style.color = color;
}

function updateTemp(state) {
    if (isNaN(state) || state == 0) {
        color = colors.disabled;
        $('#temperature-icon').removeClass('fa-thermometer-quarter');
        $('#temperature-icon').removeClass('fa-thermometer-half');
        $('#temperature-icon').removeClass('fa-thermometer-three-quarters');
        $('#temperature-icon').addClass('fa-thermometer-empty');
    }
    else if (state < 70) {
        color = colors.blue;
        $('#temperature-icon').removeClass('fa-thermometer-empty');
        $('#temperature-icon').removeClass('fa-thermometer-half');
        $('#temperature-icon').removeClass('fa-thermometer-three-quarters');
        $('#temperature-icon').addClass('fa-thermometer-quarter');
    } 
    else if (state < 80) {
        color = colors.orange;
        $('#temperature-icon').removeClass('fa-thermometer-empty');
        $('#temperature-icon').removeClass('fa-thermometer-quarter');
        $('#temperature-icon').removeClass('fa-thermometer-three-quarters');
        $('#temperature-icon').addClass('fa-thermometer-half');
    }
    else {
        color = colors.red;
        $('#temperature-icon').removeClass('fa-thermometer-empty');
        $('#temperature-icon').removeClass('fa-thermometer-quarter');
        $('#temperature-icon').removeClass('fa-thermometer-half');
        $('#temperature-icon').addClass('fa-thermometer-three-quarters');
    }
    document.getElementById('temperature').innerHTML = state + "F";
    document.getElementById('temperature-icon').style.color = color;
}

// Execute updateValues() on load of webpage
$(document).ready(updateValues())
// Execute updateValues() every 300 milliseconds after webpage is loaded
setInterval(updateValues, 1000);