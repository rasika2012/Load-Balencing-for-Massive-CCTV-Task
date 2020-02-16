
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


$(document).ready(function(){
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

});

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