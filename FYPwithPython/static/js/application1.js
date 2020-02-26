
// $(document).ready(function(){
//     //connect to the socket server.
//     var socket = io.connect('http://' + document.domain + ':' + location.port + '/test');
//     var numbers_received = [];

//     // receive details from server
//     socket.on('newnumber', function(msg) {
//         console.log("Received number" + msg.number);
//         //maintain a list of ten numbers
//         console.log(numbers_received.length)
//         if (numbers_received.length >= 20){
//             numbers_received.shift()
//         }            
//         numbers_received.push(msg.number);
//         numbers_string = '';
//         for (var i = 0; i < numbers_received.length; i++){
//             numbers_string = numbers_string + '<p>' + numbers_received[i].toString() + '</p>';
//         }
//         $('#log').html(msg.number);
//     });

// });cameraProcess


function readd(){
    //connect to the socket server.
    var socket = io.connect('http://' + document.domain + ':' + location.port + '/test');
    var camera_received = [];
    var weight_received = [];
    var count1 = 0;
    var count2 = 0;
    var count3 = 0;

    // receive details from server
    socket.on('process', function(msg) {
        console.log("Received number" + msg.camera + " "+msg.weight);
        //maintain a list of ten numbers
        console.log(camera_received.length)
        updateChart(msg);
        // if (camera_received.length >= 20){
        //     camera_received.shift()
        // }            
        camera_received.push(msg.camera);
        weight_received.push(msg.weight)
        if(msg.weight == 'GPU1'){
            count1+=1;
            processGPU1(count1)
            // console.log(count1)
            $('#gpu1').html(count1);
        }
        else if(msg.weight == 'GPU2'){
            count2+=1;
            processGPU2(count2)
            $('#gpu2').html(count2);
        }
        else if(msg.weight == 'GPU3'){
            count3+=1;
            processGPU3(count3)
            $('#gpu3').html(count3);
        }
        numbers_string = '';
        for (var i = 0; i < camera_received.length; i++){
            numbers_string = numbers_string + '<p>' + camera_received[i].toString() + '</p>';
        }
        $('#log').html(msg.camera);
        $('#ce').html(msg.weight);
    });

}

function processGPU1(count) {
    var elem1 = document.getElementById("bar1");
    elem1.style.width = count + "%";
    elem1.innerHTML = count  + "%";
}

function processGPU2(count) {
    var elem2 = document.getElementById("bar2");
    elem2.style.width = count + "%";
    elem2.innerHTML = count  + "%";
}
function processGPU3(count) {
    var elem3 = document.getElementById("bar3");
    elem3.style.width = count + "%";
    elem3.innerHTML = count  + "%";
}


// $(document).ready(function(){
//     //connect to the socket server.
//     var socket = io.connect('http://' + document.domain + ':' + location.port + '/test');

//     // receive details from server
//     socket.on('process', function(msg) {
//         console.log("Received number" + msg.camera + " "+msg.weight);
//         // window.onload = function () {

//             var dps = []; // dataPoints
//             chartcon = document.getElementById('chartContainer')
//             var chart = new CanvasJS.Chart(chartcon, {
//                 title :{
//                     text: "Dynamic Data"
//                 },
//                 axisY: {
//                     includeZero: false
//                 },      
//                 data: [{
//                     type: "line",
//                     dataPoints: dps
//                 }]
//             });
            
//             var xVal = 0;
//             var yVal = 100; 
//             var updateInterval = 1000;
//             var dataLength = 20; // number of dataPoints visible at any point
            
//             var updateChart = function (count) {
            
//                 count = count || 1;
            
//                 for (var j = 0; j < count; j++) {
//                     yVal = yVal +  Math.round(5 + Math.random() *(-5-5));
//                     dps.push({
//                         x: msg.weight,
//                         y: msg.weight
//                     });
//                     xVal++;
//                 }
            
//                 if (dps.length > dataLength) {
//                     dps.shift();
//                 }
            
//                 chart.render();
//             };
            
//             updateChart(dataLength);
//             setInterval(function(){updateChart()}, updateInterval);
            
            
//         $('#log').html(msg.camera);
//         $('#ce').html(msg.weight);
//     });

// });


// function hh(){
//         var dps = []; // dataPoints
//         var chart = new CanvasJS.Chart("chartContainer", {
//             title :{
//                 text: "Dynamic Data"
//             },
//             axisY: {
//                 includeZero: false
//             },      
//             data: [{
//                 type: "line",
//                 dataPoints: dps
//             }]
//         });
        
//         var xVal = 0;
//         var yVal = 100; 
//         var updateInterval = 1000;
//         var dataLength = 20; // number of dataPoints visible at any point
        
//         var updateChart = function (count) {
        
//             count = count || 1;
        
//             for (var j = 0; j < count; j++) {
//                 yVal = yVal +  Math.round(5 + Math.random() *(-5-5));
//                 dps.push({
//                     x: xVal,
//                     y: yVal
//                 });
//                 xVal++;
//             }
        
//             if (dps.length > dataLength) {
//                 dps.shift();
//             }
        
//             chart.render();
//         };

//         var updateChart2 = function (count) {
        
//             count = count || 1;
        
//             for (var j = 0; j < count; j++) {
//                 yVal = yVal +  Math.round(10 + Math.random() *(-4-5));
//                 dps.push({
//                     x: xVal,
//                     y: yVal
//                 });
//                 xVal++;
//             }
        
//             if (dps.length > dataLength) {
//                 dps.shift();
//             }
        
//             chart.render();
//         };
        
//         updateChart(dataLength);
//         updateChart2(dataLength);
//         setInterval(function(){updateChart()}, updateInterval);
//         setInterval(function(){updateChart2()}, updateInterval);
        
//     }


window.onload = function () {

var dataPoints1 = [];
var dataPoints2 = [];
var dataPoints3 = [];


var chart = new CanvasJS.Chart("chartContainer", {
	zoomEnabled: true,
	title: {
		text: "Share Vanies"
	},
	axisX: {
		title: "chart updates every 3 secs"
	},
	axisY:{
		prefix: "$",
		includeZero: false
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
        { 
            type: "line",
            xValueType: "dateTime",
            yValueFormatString: "$####.00",
            xValueFormatString: "hh:mm:ss TT",
            showInLegend: true,
            name: "Company A",
            dataPoints: dataPoints1
		},
		{				
			type: "line",
			xValueType: "dateTime",
			yValueFormatString: "$####.00",
			showInLegend: true,
			name: "Company B" ,
			dataPoints: dataPoints2
        },
        {				
			type: "line",
			xValueType: "dateTime",
			yValueFormatString: "$####.00",
			showInLegend: true,
			name: "Company B" ,
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
// initial value
var yValue1 = 600; 
var yValue2 = 605;

var time = new Date;
// starting at 9.30 am
time.setHours(9);
time.setMinutes(30);
time.setSeconds(00);
time.setMilliseconds(00);

function updateChart(count) {
    console.log('count '+count.camera+'count '+count.weight);
}

	
this.readd();
// setInterval(function(){updateChart()}, updateInterval);

}