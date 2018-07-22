var finish_room = false
var check= ""
var post = ""
var recieve;
var sendMes;
var revSwitch;
var revCurrent;
var revMotor;

function send_server(message,keys){
    $.ajax({
        type: "POST",
        url: "http://ecourse.cpe.ku.ac.th/exceed/api/iPalm-" + keys + "/set",
        data: {
            value: Number(message)
        },
        dataType: "text",
        success: function (response) {
            post = message
            sendMes = keys
            console.log(response)
            console.log("send complete : "+message)
        }
    });
}

$(function(){
    var recieve_server =function(add,message){
        $.ajax({
            type: "GET",
            url: "http://ecourse.cpe.ku.ac.th/exceed/api/iPalm-"+add+"/view",
            dataType: "text",
            success: function (response) {
                console.log("response1 : " + add + response)
                if(post != response){
                    console.log("response2 : " + add + response)
                    recieve = Number(response)
                    check = message
                    
                    console.log("check2:"+ check)
            }

            if(message == "switch") revSwitch = Number(response)
            else if(message == "current room") revCurrent = Number(response)
            else if(message == "motor status") revMotor = Number(response)

        },
        // timeout: 5000,
        fail: function(response){
            console.log(response) 
        }
    })};

    $('#go_room_1').on('click',function(){
        keys = "goRoom" 
        message = 1
        send_server(1,"goRoom")
        console.log(keys)
        box("กำลังไปห้อง A1")
    })
    $('#back_laundry').on('click',function(){
        keys = "goRoom"
        message = 0
        send_server(0,"goRoom")
        console.log("backLaundry")
        box("กำลังไปห้องซักรีด")
    })
    $('#stop').on('click',function(){
        keys = "motorStatus"
        message = 0
        send_server(0,"motorStatus")
        console.log(keys)
        box("พร้อมใช้งาน")
    })
    var box = function(status){
        $('#box_status').html(`สถานะ : ${status}`)
    }
    var first_time =true
    var msg = ["current room","switch","motor status"]//,"go room"]  //switch motor
    var address = ["curRoom","switchStatus","motorStatus"]//,"goRoom"]

    setInterval(function(){
        console.log("before loop "+ check)
        if(first_time == true){
            // send_server("0","goRoom")
            send_server(0,"motorStatus")
            send_server(1,"switchStatus")
            // send_server("0","curRoom")
            first_time = false
        }
        // console.log("in refresh "+ check )
        // console.log("end check server")
        // recieve_server(address[0],msg[0])
        
        var i = 0
        while(i < 3) {
            
            console.log("i : "+i)
            console.log("in refresh "+ check )
            console.log("end check server")
            // if (check ==""){
            recieve_server(address[i],msg[i])       
                                               
            console.log("check1 : " + check)
            let keys = ""
            console.log("rev" + recieve)
            if(check == "switch" && revSwitch==0){
                message = "กำลังกลับไปห้องซักรีด"
                box(message)
                console.log("POST switch ,status : finish xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
                send_server(0,"goRoom")
                send_server(1,"motorStatus")
            }

        // else if(check == "go room")
        // {
        //     keys="goRoom"

        //     message = "กำลังไปห้อง " + message 
        //     box(message)
        //     console.log(`POST go room ${message},status : `)
        //     send_server(message,keys)
        // }
        
            else if(check == "motor status"){
                if (revMotor==0 && revSwitch == 1){
                    message = "พร้อมใช้งาน"
                    box(message)
                    console.log(`POST motor ,status : ${message}`)
                }
                else if(revMotor==1 && revSwitch==1 ){
                    message = "กำลังทำงาน"
                    box(message)
                    console.log(`POST motor ,status : ${message}`)
                }

            }
        
            else if(check == "current room"){
                if (revCurrent == 2){
                    message = "อยู่ที่ห้องซักรีด"
                    writeFile("Laundry")
                    box(message)
                    console.log(`POST current room ,status : ${message}`)        
                }
                else if(revCurrent ==1){
                    message = "อยู่ที่ห้อง A1"
                    writeFile("A1")
                    box(message)
                    console.log(`POST current room ,status : ${message}`)        
                } 
        }
        i+=1
    }
    },5000)
})
