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
            // Insert variables into HTML
            // TODO: Split code up by element instead of by action
            document.getElementById('nightLight').innerHTML = nl;
            document.getElementById('airConditioning').innerHTML = ac;
            document.getElementById('smartSwitch').innerHTML = ss;
            document.getElementById('temperature').innerHTML = temp + "F";
            document.getElementById('intruderAlert').innerHTML = intruder;
            // Change Element colors in HTML
            document.getElementById('nightLight-icon').style.color = updateIcons('nl',nl);
            document.getElementById('airConditioning-icon').style.color = updateIcons('ac',ac);
            document.getElementById('smartSwitch-icon').style.color = updateIcons('ss',ss);
            document.getElementById('temperature-icon').style.color = updateIcons('temp',temp);
            document.getElementById('intruderAlert-icon').style.color = updateIcons('ia',intruder);
            // Update all icons
            updateAC(ac);
            updateSS(ss);
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

function updateAC(state) {
    if (state == "On") {
        $('#airConditioning-icon').addClass('fa-spin');
    }
    else {
        $('#airConditioning-icon').removeClass('fa-spin');
    }
}

function updateSS(state) {
    if (state == "On") {
        $('#smartSwitch-icon').removeClass('fa-toggle-off').addClass('fa-toggle-on');
    }
    else {
        $('#smartSwitch-icon').removeClass('fa-toggle-on').addClass('fa-toggle-off');
    }
}

function updateTemp(state) {
    if (isNaN(state) || state == 0) {
        color = colors.disabled;
    }
    else if (state < 70) {
        color = colors.green;
    } 
    else if (state < 80) {
        color = colors.yellow;
    }
    else {
        color = colors.red;
    }
    break;
}

function updateIcons(icon,state) {
    let colors = {
        red: "#DC3545", // old red: "#C23127",
        yellow: "#FFC107", // old yellow: "#FFCC02",
        green: "#28A745", // old green: "#4BD964",
        disabled: "#6C757D", // old disabled: "#CCCCCC",
        error: "#E83E8C" // old error: "#FF0080"
    }
    let color = "";
    switch (icon) {
        case 'nl':
            if (state == "On") {
                color = colors.yellow;
            } 
            else {
                color = colors.disabled;
            }
            break;
        case 'ac':
            if (state == "On") {
                color = colors.green;
            } 
            else {
                color = colors.disabled;
            }
            break;
        case 'ss':
            if (state == "On") {
                color = colors.green;
            } 
            else {
                color = colors.disabled;
            }
            break;
        case 'temp':
            if (isNaN(state) || state == 0) {
                color = colors.disabled;
            }
            else if (state < 70) {
                color = colors.green;
            } 
            else if (state < 80) {
                color = colors.yellow;
            }
            else {
                color = colors.red;
            }
            break;
        case 'ia':
            if (state == "On") {
                color = colors.red;
            } 
            else {
                color = colors.disabled;
            }
            break;
        default:
            color = color.error;
            break;
    }
    return color;
}

// Execute updateValues() on load of webpage
$(document).ready(updateValues())
// Execute updateValues() every 300 milliseconds after webpage is loaded
setInterval(updateValues, 1000);