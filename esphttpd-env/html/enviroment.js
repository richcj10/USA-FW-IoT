var red = 0;
var green = 0;
var blue = 0;

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
