var msg = ["switch","go room","motor status","current room"]  //switch motor
var address = ["switchStatus","goRoom","motorStatus","curRoom"]
var finish_room = false
// var recieve = ""
var check=""

$(function(){
    var message =""
    var post =""
    var cur_room=""
    var add_now =""
    var add_old =""
    var value_now =""
    var value_old =""
    
    var recieve_server =function(add,msg){
        add_now = add
        $.ajax({
        type: "GET",
        url: "http://ecourse.cpe.ku.ac.th/exceed/api/iPalm-"+add_now+"/view",
        dataType: "text",
        success: function (response) {
            console.log("recieve"+ add_now)
            if (add_old != add_now && post != response){
                console.log("get "+ msg)
                console.log("response" + response)                   
                check = msg
                value_now = response
                cur_room = value_now
                add_old = add_now
            }
            else if(value_now != value_old){
                console.log("get "+ msg)
                console.log("response" + response)    
                check = msg
                cur_room = value_now
                add_old = add_now
            }
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
                post = message
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
        box("หยุดทำงาน")
    })
    var box = function(status){
    $('#box_status').html(`สถานะ : ${status}`)
    }
    var first_time =true
    setInterval(function(){ 
        let status = ""
        var i = 0       
    
        console.log("before loop "+ check)
        if(first_time == true){
            send_server("0","goRoom")
            send_server("0","motorStatus")
            send_server("0","switchStatus")
            send_server("0","curRoom")
            first_time = false
        }
        while(i < 4) {
            console.log("in refresh "+ check )
            console.log("end check server")
            if (check ==""){
                recieve_server(address[i],msg[i])       
            }
            i++;
            // console.log("check "+check)
        }

        console.log("add_old"+ add_old)
        console.log("add_now"+add_now)
        let keys = ""
        
        if(check == "switch"){
            // if (first_time == false){  
                message = "กลับห้อง"
                box(message)
                console.log("POST switch ,status : finish")
                send_server(0,"goRoom")
                send_server(1,"motorStatus")
        //     }
        // else{
        //     first_time = false
        // }
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
            // if (first_time == false){  
                keys="motorStatus"
                message = "หยุดทำงาน"
                box(message)
                console.log(`POST motor ,status : ${message}`)
                send_server(0,keys)
            // }
            // else{
            //     first_time = false
            // }
        }
        
        else if(check == "current room"){
            // if (first_time == false){  
                if (cur_room == "0"){
                    message = "อยู่ที่ห้องซักรีด"
                    box(message)
                    console.log(`POST current room ,status : ${message}`)
                } 
                else if(cur_room =="1"){
                    message = "อยู่ที่ห้อง A1"
                    box(message)
                    console.log(`POST current room ,status : ${message}`)        
                } 
            // }
            // else{
            //     first_time = false
            // }
            // recieve =""
        }


        // console.log("end check1 : " +check)     
        check = ""
        // console.log("end check2 : " +check)     

    },5000)
})
