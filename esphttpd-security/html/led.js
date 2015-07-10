function save(param, payload) {
	doingsave=true;
    $.ajax({
        type: 'GET',
        url: "led.cgi?" + param + "=" + payload,
        async: true,
		timeout: 3000,
		tryCount : 0,
		retryLimit : 3,
		success: function (data) {
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