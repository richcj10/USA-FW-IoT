var data_temp = {
  // A labels array that can contain any sort of values
  labels: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10],
  // Our series array that contains series objects or in this case series data arrays
  series:[ [70, 70, 70, 70, 70,70, 70, 70, 70, 70] ]
};

var data_hud = {
  // A labels array that can contain any sort of values
   labels: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10],
  // Our series array that contains series objects or in this case series data arrays
  series:[ [30, 30, 30, 30, 30,30, 30, 30, 30, 30] ]
};

var options1 = {
  // If high is specified then the axis will display values explicitly up to this value and the computed maximum from the data is ignored
  high: 85,
  // If low is specified then the axis will display values explicitly down to this value and the computed minimum from the data is ignored
  low: 0,
  // If specified then the value range determined from minimum to maximum (or low and high) will be divided by this number and ticks will be generated at those division points. The default divisor is 1.
  divisor: 10,
};

var options2 = {
  // If high is specified then the axis will display values explicitly up to this value and the computed maximum from the data is ignored
  high: 75,
  // If low is specified then the axis will display values explicitly down to this value and the computed minimum from the data is ignored
  low: 0,
  // If specified then the value range determined from minimum to maximum (or low and high) will be divided by this number and ticks will be generated at those division points. The default divisor is 1.
  divisor: 10,
};

var connected = 0;

window.onload=function(){
    
// Create a new line chart object where as first parameter we pass in a selector
// that is resolving to our chart container element. The Second parameter
// is the actual data object.
new Chartist.Line('.temp-chart', data_temp, options1);
new Chartist.Line('.hudm-chart', data_hud, options2);
 
}

setInterval(function(){updateData()},1000);

function updateData() {
    data_temp.series[0][9] = data_temp.series[0][8];
    data_temp.series[0][8] = data_temp.series[0][7];
    data_temp.series[0][7] = data_temp.series[0][6];
    data_temp.series[0][6] = data_temp.series[0][5];
    data_temp.series[0][5] = data_temp.series[0][4];
    data_temp.series[0][4] = data_temp.series[0][3];
    data_temp.series[0][3] = data_temp.series[0][2];
    data_temp.series[0][2] = data_temp.series[0][1];
    data_temp.series[0][1] = data_temp.series[0][0];
    data_hud.series[0][9] = data_hud.series[0][8];
    data_hud.series[0][8] = data_hud.series[0][7];
    data_hud.series[0][7] = data_hud.series[0][6];
    data_hud.series[0][6] = data_hud.series[0][5];
    data_hud.series[0][5] = data_hud.series[0][4];
    data_hud.series[0][4] = data_hud.series[0][3];
    data_hud.series[0][3] = data_hud.series[0][2];
    data_hud.series[0][2] = data_hud.series[0][1];
    data_hud.series[0][1] = data_hud.series[0][0];
    server_get();
    server_get2();
    new Chartist.Line('.temp-chart', data_temp,options1);
    new Chartist.Line('.hudm-chart', data_hud,options2);
    console.log(data_hud.series[0]);
    console.log(data_temp.series[0]);
}

function server_get() {
    var output = {};
		$.ajax({
			url: "chartdata.cgi?param=temp",
			dataType: 'json',
			async: true,
			timeout: 3000,
			tryCount : 0,
			retryLimit : 1,				success: function (data) {
				if (data.length !== 0) {
					statusMsg = false;
					//if(!connected) setStatus("Connected",2,0); 
					connected=true;
				    data_temp.series[0][0] = parseInt(data);
                    console.log("%s",output);
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
    return output;
}

function server_get2() {
    var output = {};
		$.ajax({
			url: "chartdata.cgi?param=hud",
			dataType: 'json',
			async: true,
			timeout: 3000,
			tryCount : 0,
			retryLimit : 1,				success: function (data) {
				if (data.length !== 0) {
					statusMsg = false;
					//if(!connected) setStatus("Connected",2,0); 
					connected=true;
                    data_hud.series[0][0] = parseInt(data);
                    console.log("%s",output);
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
    return output;
}
