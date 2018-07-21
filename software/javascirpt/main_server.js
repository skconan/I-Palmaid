var button_go_room_1 = 0
var button_back_laundry = 0

var msg = ["switch","go room","motor status"]
var address = ["switchStatus","goRoom","motorStatus"]
var finish_room = false

$(function(){
    var message =""
    var post =""
    var recieve = ""
    var check=""

    
    var recieve_server =function(address,msg){
        $.ajax({
        type: "GET",
        url: "http://ecourse.cpe.ku.ac.th/exceed/api/iPalm-"+address+"/view",
        dataType: "text",
        success: function (response) {
            if (recieve != response && post != response){
                console.log("get "+ msg)
                console.log(response)    
                check = msg
                recieve = response
            } 
        },
        fail: function(response){
            console.log(response) 
        }
    })};
    var send_server = function(message,keys){
        $.ajax({
            type: "POST",
            url: "http://ecourse.cpe.ku.ac.th/exceed/api/iPalm-" + keys + "/set",
            data: {
                value: Number(message)
            },
            dataType: "text",
            success: function (response) {
                // post = message
                console.log(response)
                console.log("send complete")
            }
        });
    }
    $('#go_room_1').on('click',function(){
        keys = "goRoom" 
        message = 1
        send_server(1,keys)
        box("กำลังไปห้องเบอร์ 1")
    })
    $('#back_laundry').on('click',function(){
        keys = "goRoom"
        message = 0
        send_server(0,keys)
        box("กำลังไปห้องซักรีด")
    })
    $('#stop').on('click',function(){
        keys = "motorStatus"
        message = 0
        send_server(0,keys)
        box("หยุดทำงาน")
    })
    var box = function(status){
    $('#box_status').html(`สถานะ : <p>${status}</p>`)
    }
    var i = 0
    var first_time =true
    setInterval(function(){ 
        let status = ""
        
        console.log("before loop "+ check )
      
        ////reset value on server

        if (first_time==true){
            send_server(" ",address[0])
            send_server(" ",address[1])
            send_server(" ",address[2])
            first_time = false
            console.log("reset value")
        }
           

        while(i < 3) {
            console.log("in refresh "+ check )
            console.log("end check server")
            if (check ==""){
                recieve_server(address[i],msg[i])       
            }
            i++;
        }
        let keys = ""
        
        if(check == "switch"){
            keys= "switchStatus"
            message = "ทำงานเสร็จ"
            box(message)
            console.log("POST switch ,status : finish")
        }
        else if(check == "go room")
        {
            keys="goRoom"
            if (button_go_room_1 == "0"){
                button_go_room_1 = 1
            }
            else {
                button_go_room_1 = 0
            }
            message = "กำลังไปห้อง " + message 
            box(message)
            console.log(`POST go room ${message},status : ${button_go_room_1}`)
            send_server(message,keys)
        }
        else if(check == "motor status")
        {

            keys="motorStatus"
            if (button_go_room_1 == "0"){
                button_go_room_1 = 1
            }
            else {
                button_go_room_1 = 0
            }
            message = "หยุดทำงาน"
            box(message)
            console.log(`POST motor ,status : ${message}`)
            send_server(message,keys)
        }
        check = ""
        console.log("end check : " +check)     
    },5000)
})
