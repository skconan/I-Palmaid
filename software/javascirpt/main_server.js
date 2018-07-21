var button_go_room_1 = 0
var button_back_laundry = 0

var msg = ["switch","go room","motor status"]
var address = ["switchStatus","goRoom","motorStatus"]
var finish_room = false

$(function(){
    var check="";
    var recieve_server =function(address,msg){
        $.ajax({
        type: "GET",
        url: "http://ecourse.cpe.ku.ac.th/exceed/api/iPalm-"+address+"/view",
        dataType: "text",
        success: function (response) {
            console.log("get "+ msg)
            console.log(response)
            if (response !=""){
            check = msg
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
                value: message
            },
            dataType: "text",
            success: function (response) {
                console.log(response)
            }
        });
    }
    $('#go_room_1').on('click',function(){
        check = "go room" 
        message = 1
    })
    $('#back_laundry').on('click',function(){
        check = "back laundry"
        message = 0
    })
    $('#stop').on('click',function(){
        check = "motor status"
        message = 0
    })
    var box = function(status){
    $('#box_status').html(`สถานะ : <h1>${status}</h1>`)
    }
    setInterval(function(){ 
        let status = ""
        let i = 0
        console.log("before loop "+ check )
        while (i < 3) {
            if (check ==""){
                recieve_server(address[i],msg[i])         
            }
            i++;
        }
        console.log("in refresh "+ check )
        console.log("end check server")

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
            message = "กำลังไปห้อง " + massage 
            box(massage)
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
            massage = "หยุดทำงาน"
            box(massage)
            console.log(`POST motor ,status : ${message}`)
            send_server(message,keys)
        }
    })
})
