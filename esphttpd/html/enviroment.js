var now = new Date();
var timenow = now.getHours() + (now.getMinutes() / 60);
var days = {
    0: 'sun',
    1: 'mon',
    2: 'tue',
    3: 'wed',
    4: 'thu',
    5: 'fri',
    6: 'sat',
    7: 'sun'
};
var today = days[now.getDay()];

//=================================================
// DATA
//=================================================

var visibleFlag = 1;
var setpoint = 21;
var unit ="&deg;C";
var statusMsg = false;
var connected = false;
var doingsave = false;

var thermostat = {
	temperature: "21",
	humidity: "50",
	humidistat: 0,
	relay1state: 0,
	relay1name: "Zone Name",
	opmode: 0,
    state: 0,
    manualsetpointred: 50,
    manualsetpointgreen: 50,
    manualsetpointblue: 50,
    mode: 0
};

var schedule = {};

var maxc = 100;
var minc = 0;
// ================================================
// State variables
// ================================================
var editmode = 'move';
$("#mode-move").css("background-color", "#ff9600");
var key = 1;
var day = "mon";
var mousedown = 0;
var slider_width = $(".slider").width();
var slider_height = $(".slider").height();
var changed = 0;

thermostat.manualsetpoint/=100;
		
$("#unit").html(unit);
			
setpoint = thermostat.manualsetpointgreen;
$(".green-setpoint").html(setpoint.toFixed(1) + unit);
setpoint = thermostat.manualsetpointred;
$(".red-setpoint").html(setpoint.toFixed(1) + unit);
setpoint = thermostat.manualsetpointblue;
$(".blue-setpoint").html(setpoint.toFixed(1) + unit);


function setStatus(msg,dur,pri){	 // show msg on status bar
		if(statusMsg == true){return};
		statusMsg= true;
		if(pri>0){
			$("#statusView").toggleClass("statusViewAlert",true);
			$("#statusView").toggleClass("statusView",false);
		} else {
			$("#statusView").toggleClass("statusView",true);
			$("#statusView").toggleClass("statusViewAlert",false);
		}
		$("#statusView").show();
		$("#statusView").html(msg);
		dur = dur*1000;
		if(dur >0){
			setTimeout(function(){$("#statusView").hide(200);$("#statusView").html(""); statusMsg= false},dur)
		}
	}

function update() {

	$(".zone-title").html(thermostat.relay1name);

	if(!isNaN((Number(thermostat.humidity)).toFixed(1)))
		$('.humidity').show();
	
	$(".zone-temperature").html((Number(thermostat.temperature)).toFixed(1) + "&deg;C");
    $("#zone-humidity").html((Number(thermostat.humidity)).toFixed(1) + "%");
	
	
	if (thermostat.mode === 0) {
		$(".thermostatmode").css("background-color", "#555");
		$("#manual_thermostat").css("background-color", "#ff9600");
		$("#scheduled_thermostat").css("background-color", "#555");
	} else {
		$(".thermostatmode").css("background-color", "#555");
		$("#manual_thermostat").css("background-color", "#555");
		$("#scheduled_thermostat").css("background-color", "#ff9600");
	}
	
	if (thermostat.opmode == 0) {
		$(".thermostatopmode").css("background-color", "#555");
		$("#heating_thermostat").css("background-color", "#c00000");
		$("#cooling_thermostat").css("background-color", "#555");
	} else {
		$(".thermostatopmode").css("background-color", "#555");    
		$("#heating_thermostat").css("background-color", "#555");
		$("#cooling_thermostat").css("background-color", "#0000c0");
	}
	
}
	
$("#toggle").click(function () {
    thermostat.state++;
    if (thermostat.state > 1) thermostat.state = 0;
    if (thermostat.state == 1) {
        $("#toggle").html("ON");
        $(this).css("background-color", "#ff9600");
    }
    if (thermostat.state === 0) {
        $("#toggle").html("OFF");
        $(this).css("background-color", "#555");
    }

    //save("tx/heating",thermostat.state+","+parseInt(setpoint*100));
    save("thermostat_state", thermostat.state.toString());
});

$("#red-setpoint-dec").click(function () {
    $(".thermostatmode").css("background-color", "#555");
    $("#manual_thermostat").css("background-color", "#ff9600");
    if(thermostat.manualsetpointred >= 5){
        thermostat.manualsetpointred -= 5;
    }
    setpoint = thermostat.manualsetpointred;
    $(".red-setpoint").html(setpoint.toFixed(1) + unit);
    save("red_manualsetpoint", ((thermostat.manualsetpointred.toFixed(1)) * 100).toString());
});

$("#red-setpoint-inc").click(function () {
    $(".thermostatmode").css("background-color", "#555");
    $("#manual_thermostat").css("background-color", "#ff9600");
    if(thermostat.manualsetpointred <= 95){
        thermostat.manualsetpointred += 5;
    }
    setpoint = thermostat.manualsetpointred;
    $(".zone-setpoint").html(setpoint.toFixed(1) + unit);
    save("red_manualsetpoint", ((thermostat.manualsetpointred.toFixed(1)) * 100).toString());
});

$("#blue-setpoint-dec").click(function () {
    $(".thermostatmode").css("background-color", "#555");
    $("#manual_thermostat").css("background-color", "#ff9600");
    if(thermostat.manualsetpointblue >= 5){
        thermostat.manualsetpointblue -= 5;
    }
    
    setpoint = thermostat.manualsetpointblue;
    $(".blue-setpoint").html(setpoint.toFixed(1) + unit);
    save("blue_manualsetpoint", ((thermostat.manualsetpointblue.toFixed(1)) * 100).toString());
});

$("#blue-setpoint-inc").click(function () {
    $(".thermostatmode").css("background-color", "#555");
    $("#manual_thermostat").css("background-color", "#ff9600");
    if(thermostat.manualsetpointblue <= 95){
        thermostat.manualsetpointblue += 5;
    }
    setpoint = thermostat.manualsetpointblue;
    $(".blue-setpoint").html(setpoint.toFixed(1) + unit);
    save("blue_manualsetpoint", ((thermostat.manualsetpointblue.toFixed(1)) * 100).toString());
});

$("#green-setpoint-dec").click(function () {
    $(".thermostatmode").css("background-color", "#555");
    $("#manual_thermostat").css("background-color", "#ff9600");
    if(thermostat.manualsetpointgreen >= 5){
        thermostat.manualsetpointgreen -= 5;
    }
    setpoint = thermostat.manualsetpointgreen;
    $(".green-setpoint").html(setpoint.toFixed(1) + unit);
    save("green_manualsetpoint", ((thermostat.manualsetpointgreen.toFixed(1)) * 100).toString());
});

$("#green-setpoint-inc").click(function () {
    $(".thermostatmode").css("background-color", "#555");
    $("#manual_thermostat").css("background-color", "#ff9600");
    if(thermostat.manualsetpointgreen <= 95){
        thermostat.manualsetpointgreen += 5;
    }
    setpoint = thermostat.manualsetpointgreen;
    $(".green-setpoint").html(setpoint.toFixed(1) + unit);
    save("green_manualsetpoint", ((thermostat.manualsetpointgreen.toFixed(1)) * 100).toString());
});

// MERGE
$("body").on("click", ".slider-button", function () {
    if (editmode == 'merge') {
        day = $(this).parent().attr("day");
        key = parseInt($(this).attr("key"),10);
        schedule[day][key - 1].e = schedule[day][key].e;
        schedule[day].splice(key, 1);
        draw_day_slider(day);
        //editmode = 'move';
        save("thermostat_schedule", "{\"" + day + "\":" + JSON.stringify(calc_schedule_esp(schedule[day])) + "}");
    }
});

$("body").on("click", ".slider-segment", function (e) {

    day = $(this).parent().attr("day");
    key = parseInt($(this).attr("key"),10);

    if (editmode == 'split') {
        var x = e.pageX - $(this).parent()[0].offsetLeft;
        var prc = x / slider_width;
        var hour = prc * 24.0;
        hour = Math.round(hour / 0.5) * 0.5;

        if (hour > schedule[day][key].s + 0.5 && hour < schedule[day][key].e - 0.5) {
            var end = parseFloat(schedule[day][key].e);
            schedule[day][key].e = hour;

            schedule[day].splice(key + 1, 0, {
                s: hour,
                e: end,
                sp: schedule[day][key].sp
            });

            draw_day_slider(day);
            $("#average_temperature").html(calc_average_schedule_temperature().toFixed(1));
            save("thermostat_schedule", "{\"" + day + "\":" + JSON.stringify(calc_schedule_esp(schedule[day])) + "}");
        }
        //editmode = 'move';
    } else if (editmode == 'move') {
        $("#slider-segment-temperature").val((schedule[day][key].sp * 1).toFixed(1));
        $("#slider-segment-start").val(format_time(schedule[day][key].s));
        $("#slider-segment-end").val(format_time(schedule[day][key].e));

        $("#slider-segment-block").show();
        $("#slider-segment-block-movepos").hide();
    }
});

//function slider_update(e) {
//    $("#slider-segment-block-movepos").show();
//    $("#slider-segment-block").hide();
//
//    if (key !== undefined) {
//        var x = e.pageX - $(".slider[day=" + day + "]")[0].offsetLeft;
//
//        var prc = x / slider_width;
//        var hour = prc * 24.0;
//        hour = Math.round(hour / 0.5) * 0.5;
//
//        if (hour > schedule[day][key - 1].s && hour < schedule[day][key].e) {
//            schedule[day][key - 1].e = hour;
//            schedule[day][key].s = hour;
//            update_slider_ui(day, key);
//            changed = 1;
//        }
//        $("#slider-segment-time").val(format_time(schedule[day][key].s));
//    }
//    // $("#average_temperature").html(calc_average_schedule_temperature().toFixed(1));
//
//
//}

//$("body").on("click", "#slider-segment-ok", function () {
//
//    schedule[day][key].sp = $("#slider-segment-temperature").val();
//    var color = color_map(schedule[day][key].sp);
//    $(".slider[day=" + day + "]").find(".slider-segment[key=" + key + "]").css("background-color", color);
//
//    var time = decode_time($("#slider-segment-start").val());
//    if (time != -1 && key > 0 && key < schedule[day].length) {
//        if (time >= (schedule[day][key - 1].s + 0.5) && time <= (schedule[day][key].e - 0.5)) {
//            schedule[day][key - 1].e = time;
//            schedule[day][key].s = time;
//        }
//    }
//    $("#slider-segment-start").val(format_time(schedule[day][key].s));
//    update_slider_ui(day, key);
//
//    time = decode_time($("#slider-segment-end").val());
//    if (time != -1 && key > 0 && key < (schedule[day].length - 1)) {
//        if (time >= (schedule[day][key].s + 0.5) && time <= (schedule[day][key + 1].e - 0.5)) {
//            schedule[day][key].e = time;
//            schedule[day][key + 1].s = time;
//        }
//    }
//    $("#slider-segment-end").val(format_time(schedule[day][key].e));
//    update_slider_ui(day, key + 1);
//    save("thermostat_schedule", "{\"" + day + "\":" + JSON.stringify(calc_schedule_esp(schedule[day])) + "}");
//    updateclock();
//
//});

//$("#slider-segment-movepos-ok").click(function () {
//
//    var time = decode_time($("#slider-segment-time").val());
//    if (time != -1 && key > 0) {
//        if (time >= (schedule[day][key - 1].s + 0.5) && time <= (schedule[day][key].e - 0.5)) {
//            schedule[day][key - 1].e = time;
//            schedule[day][key].s = time;
//        }
//    }
//    $("#slider-segment-time").val(format_time(schedule[day][key].s));
//    update_slider_ui(day, key);
//    save("thermostat_schedule", "{\"" + day + "\":" + JSON.stringify(calc_schedule_esp(schedule[day])) + "}");
//});
//
//$("#mode-split").click(function () {
//    editmode = 'split';
//    $(".editmode").css("background-color", "#555");
//    $(this).css("background-color", "#ff9600");
//});
//
//
//$("#mode-move").click(function () {
//    editmode = 'move';
//    $(".editmode").css("background-color", "#555");
//    $(this).css("background-color", "#ff9600");
//});
//
//$("#mode-merge").click(function () {
//    editmode = 'merge';
//    $(".editmode").css("background-color", "#555");
//    $(this).css("background-color", "#ff9600");
//});

//$("#manual_thermostat").click(function () {
//    $(".thermostatmode").css("background-color", "#555");
//    $(this).css("background-color", "#ff9600");
//    thermostat.mode = 0;
//	
//	setpoint = thermostat.manualsetpoint;
//	$(".zone-setpoint").html(setpoint.toFixed(1) + unit);
//
//    save("thermostat_mode", (thermostat.mode).toString());
//    updateclock();
//});
//
//$("#scheduled_thermostat").click(function () {
//    $(".thermostatmode").css("background-color", "#555");
//    $(this).css("background-color", "#ff9600");
//    thermostat.mode = 1;
//    save("thermostat_mode", (thermostat.mode).toString());
//    updateclock();
//});
//
//$("#heating_thermostat").click(function () {
//    $(".thermostatopmode").css("background-color", "#555");
//    $(this).css("background-color", "#c00000");
//    opmode = 0;
//    save("thermostat_opmode", opmode.toString());
//    updateclock();
//});
//
//$("#cooling_thermostat").click(function () {
//    $(".thermostatopmode").css("background-color", "#555");
//    $(this).css("background-color", "#0000c0");
//    opmode = 1;
//    save("thermostat_opmode", opmode.toString());
//    updateclock();
//});

function color_map(temperature) {
    /*
    // http://www.particleincell.com/blog/2014/colormap/
    // rainbow short
    var f=(temperature-minc)/(maxc-minc);	//invert and group
	var a=(1-f)/0.25;	//invert and group
	var X=Math.floor(a);	//this is the integer part
	var Y=Math.floor(255*(a-X)); //fractional part from 0 to 255
	switch(X)
	{
		case 0: r=255;g=Y;b=0;break;
		case 1: r=255-Y;g=255;b=0;break;
		case 2: r=0;g=255;b=Y;break;
		case 3: r=0;g=255-Y;b=255;break;
		case 4: r=0;g=0;b=255;break;
	}
     
	*/
    var f = (temperature - minc) / (maxc - minc);
    var a = (1 - f);
    var Y = Math.floor(255 * a);
    r = 255;
    g = Y;
    b = 0;

    return "rgb(" + r + "," + g + "," + b + ")";
}

function update_slider_ui(day, key) {
    if (schedule[day] !== undefined && key < schedule[day].length) {
        var slider = $(".slider[day=" + day + "]");
        if (key > 0) {
            var width = ((schedule[day][key - 1].e - schedule[day][key - 1].s) / 24.0) * 100;
            slider.find(".slider-segment[key=" + (key - 1) + "]").css("width", width + "%");
        }

        var left = (schedule[day][key].s / 24.0) * 100;
        var width = ((schedule[day][key].e - schedule[day][key].s) / 24.0) * 100;
        slider.find(".slider-segment[key=" + key + "]").css("width", width + "%");
        slider.find(".slider-segment[key=" + key + "]").css("left", left + "%");
        slider.find(".slider-button[key=" + key + "]").css("left", left + "%");
    }
}

function format_time(time) {
    var hour = Math.floor(time);
    var mins = Math.round((time - hour) * 60);
    if (mins < 10) mins = "0" + mins;
    return hour + ":" + mins;
}

function decode_time(timestring) {
    var time = -1;
    if (timestring.indexOf(":") != -1) {
        var parts = timestring.split(":");
        var hour = parseInt(parts[0],10);
        var mins = parseInt(parts[1],10);

        if (mins >= 0 && mins < 60 && hour >= 0 && hour < 25) {
            if (hour == 24 && mins !== 0) {} else {
                time = hour + (mins / 60);
            }
        }
    }
    return time;
}

function calc_average_schedule_temperature() {
    var sum = 0;
    for (var d in schedule) {
        for (var z in schedule[d]) {
            var hours = (schedule[d][z].e - schedule[d][z].s)
            sum += (schedule[d][z].sp * hours);
        }
    }
    return sum / (24 * 7.0);
}

function calc_schedule_esp(sched) {
    var fixsched = JSON.parse(JSON.stringify(sched));
    for (var d in fixsched) {
        fixsched[d].s *= 100;
        fixsched[d].e *= 100;
        fixsched[d].sp *= 100;
    }
    return fixsched;
}

// function for checking if the page is visible or not
// (if not visible it will stop updating data)
function checkVisibility() {
    $(window).bind("focus", function(event) {
        visibleFlag = 1;
    });

    $(window).bind("blur", function(event) {
        visibleFlag = 0;
    });
}

function save(param, payload) {
	doingsave=true;
    $.ajax({
        type: 'POST',
        url: "env.cgi?param=" + param,
        data: payload,
		dataType: 'text',
		cache: false,
        async: true,
			timeout: 3000,
			tryCount : 0,
			retryLimit : 3,			success: function (data) {
			statusMsg = false;
			if(!connected) setStatus("Connected",2,0); 
			connected=true;
			doingsave=false;
		},
		error : function(xhr, textStatus, errorThrown ) {
        if (textStatus == 'timeout') {
            this.tryCount++;
            if (this.tryCount <= this.retryLimit) {
                //try again
                $.ajax(this);
                return;
            }            
            return;
        }
		if(connected) setStatus("No connection to server!",0,1);
		connected=false;
		doingsave=false;
		}
    });
}

function server_get() {
    var output = {};
	if (visibleFlag) {
		$.ajax({
			url: "thermostat.cgi?param=state",
			dataType: 'json',
			async: true,
			timeout: 3000,
			tryCount : 0,
			retryLimit : 3,				success: function (data) {
				if (data.length !== 0) {
					statusMsg = false;
					if(!connected) setStatus("Connected",2,0); 
					connected=true;
					if(!doingsave) {
						output = data;
						thermostat=data;
						thermostat.manualsetpoint/=100;
						update();
					}
				}
			},
		error : function(xhr, textStatus, errorThrown ) {
        if (textStatus == 'timeout') {
            this.tryCount++;
            if (this.tryCount <= this.retryLimit) {
                //try again
                $.ajax(this);
                return;
            }            
            return;
        }
		if(connected) setStatus("No connection to server!",0,1);
		connected=false;
		}
		});
	}
    return output;
}
