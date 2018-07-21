var msg = ["switch","motor status"]//,"current room"]//,"go room"]  //switch motor
var address = ["switchStatus","motorStatus"]//,"curRoom"]//,"goRoom"]
var finish_room = false
// var recieve = ""
var check=""

$(function(){
    // var message =""
    // var cur_room=""
    var recieve =""
    // var post = ""
    // var add_old=""
    // var add_new=""
    
    var recieve_server =function(add,message){
        $.ajax({
        type: "GET",
        url: "http://ecourse.cpe.ku.ac.th/exceed/api/iPalm-"+add+"/view",
        dataType: "text",
        success: function (response) {
            recieve = response
            check = message
            // if(add==address[0] && response == "0"){
            //     check = "switch"
            //     recieve = response
            // }
            // else if(add==address[1] && response=="0"){
            //     check = "motorStatus"
            //     recieve = "0"
            // }
            // else{
            //     check = "motorStatus"
            //     recieve = "1"
            // }

        },
        // timeout: 5000,
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
                console.log("send complete : "+message)
            }
        });
    }
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
    var cur_count = false
    setInterval(function(){ 
        let status = ""
        var i = 0       
    
        console.log("before loop "+ check)
        if(first_time == true){
            // send_server("0","goRoom")
            send_server("0","motorStatus")
            send_server("1","switchStatus")
            // send_server("0","curRoom")
            first_time = false
        }
        while(i < 2) {
            console.log("in refresh "+ check )
            console.log("end check server")
            if (check ==""){
                recieve_server(address[i],msg[i])       
            }
            i++;
        }
        console.log("check "+check)
        let keys = ""
        
        if(check == "switch"){
            if(recieve = "0"){
                message = "กำลังกลับไปห้องซักรีด"
                box(message)
                console.log("POST switch ,status : finish")
                send_server(0,"goRoom")
                send_server(1,"motorStatus")
                
        }}

        // else if(check == "go room")
        // {
        //     keys="goRoom"

        //     message = "กำลังไปห้อง " + message 
        //     box(message)
        //     console.log(`POST go room ${message},status : `)
        //     send_server(message,keys)
        // }
        
        else if(check == "motor status"){
                keys="motorStatus"
                if (recieve=="0"){
                message = "พร้อมใช้งาน"
            }
                else if(recieve=="1"){
                    message = "กำลังทำงาน"
                }
                box(message)
                console.log(`POST motor ,status : ${message}`)
        }
        
        // else if(check == "current room"){
        //         if (recieve == "0"){
        //             if (cur_count ==false){
        //             message = "อยู่ที่ห้องซักรีด"
        //             cur_count = true
        //         }
        //         else if (cur_count == true){
        //             message = "กำลังทำงาน"
        //             }
        //             box(message)
        //             console.log(`POST current room ,status : ${message}`)
        //         } 
        //         else if(recieve =="1"){
        //             message = "อยู่ที่ห้อง A1"
        //             box(message)
        //             console.log(`POST current room ,status : ${message}`)        
        //         } 
        // }


        check = ""
    },5000)
})
