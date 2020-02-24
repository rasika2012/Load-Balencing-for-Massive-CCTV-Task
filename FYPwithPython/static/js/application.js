window.onload = function () {

    var dataPoints1 = [];
    var dataPoints2 = [];
    var dataPoints3 = [];
    
    
    var chart = new CanvasJS.Chart("chartContainer", {
        zoomEnabled: true,
        title: {
            text: "GPU Load"
        },
        axisX: {
            title: "chart updates every 3 secs"
        },
        axisY:{
            // prefix: "$"
            includeZero: true
        }, 
        toolTip: {
            shared: true
        },
        legend: {
            cursor:"pointer",
            verticalAlign: "top",
            fontSize: 22,
            fontColor: "dimGrey",
            itemclick : toggleDataSeries
        },
        data: [
            {   type: "line",
                xValueType: "dateTime",
                yValueFormatString: "####.00",
                xValueFormatString: "hh:mm:ss TT",
                showInLegend: true,
                name: "GPU 1",
                dataPoints: dataPoints1
            },
            {	type: "line",
                xValueType: "dateTime",
                yValueFormatString: "####.00",
                showInLegend: true,
                name: "GPU 2" ,
                dataPoints: dataPoints2
            },
            {	type: "line",
                xValueType: "dateTime",
                yValueFormatString: "####.00",
                showInLegend: true,
                name: "GPU 3" ,
                dataPoints: dataPoints3
        }]
    });

    function toggleDataSeries(e) {
        if (typeof(e.dataSeries.visible) === "undefined" || e.dataSeries.visible) {
            e.dataSeries.visible = false;
        }
        else {
            e.dataSeries.visible = true;
        }
        chart.render();
    }



    var updateInterval = 3000;
  

    var time = new Date;

    function updateChart(val1,val2,val3) {
        
      
        time.setTime(time.getTime()+ updateInterval);
      
        dataPoints1.push({
            x: time.getTime(),
            y: val1
        });
        dataPoints2.push({
            x: time.getTime(),
            y: val2
        });
        dataPoints3.push({
            x: time.getTime(),
            y: val3
        });

        if (dataPoints1.length > 20) {
            dataPoints1.shift();
            dataPoints2.shift();
            dataPoints3.shift();
        }
       
    
        // updating legend text with  updated with y Value 
        chart.options.data[0].legendText = "GPU1 Computing time";
        chart.options.data[1].legendText = "GPU2 Computing time"; 
        chart.options.data[2].legendText = "GPU3 Computing time"; 
        chart.render();
    
    }

    function runDetails(){
    //connect to the socket server.
        var socket = io.connect('http://' + document.domain + ':' + location.port + '/test');
        // receive details from server
        socket.on('process', function(msg) {
            console.log("Received number" + msg.v.GPU1);
            
            updateChart(msg.v.GPU1,msg.v.GPU2,msg.v.GPU3)
    });

    }

    
    runDetails();
    // setInterval(function(){updateChart()}, updateInterval);
}
