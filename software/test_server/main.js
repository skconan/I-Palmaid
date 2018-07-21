// var refreshIntervalId = setInterval(fname, 10000);
$(function(){
    var a=""
    var recieve = ""
    var send = function(add){
        if (a == ""){        
        a = $('#text-input').val()
        }
        $.ajax({
        type: "POST",
        url: "http://ecourse.cpe.ku.ac.th/exceed/api/iPalm-"+add +"/set",
        data: {
            value: Number(a)
        },
        dataType: "text",
        success: function (response) {
            $('#box').append(`<div align=left> <p>my ${add} : ${a} </p></div><hr>`)
            console.log(response)
            console.log("success post " + a)
            $('#text-input').val('')
            }
        });
    }
    
    var timeId = 0

    $(`#push`).on('click',function(){
        console.log("push accept")
        timeId = setInterval(function(){
            $.ajax({
                type: "GET",
                url: "http://ecourse.cpe.ku.ac.th/exceed/api/iPalm-switchStatus/view",
                dataType: "text",
                success: function (response) {
                    console.log("recieve")   
                    console.log(response)
                    $('#box').append(`<div align=right> <p> server status : ${response} </p></div><hr>`)
                    if (recieve != response){
                    recieve = response
                    $.ajax({
                        type: "POST",
                        url: "http://ecourse.cpe.ku.ac.th/exceed/api/iPalm-switchStatus/set",
                        data: {
                            value: Number(recieve)
                        },
                        dataType: "text",
                        success: function (response) {
                            console.log(response)
                            console.log("success post " + recieve)
                            }
                        });
                    }        
                }, timeout: 5000,
                fail: function(response){
                    console.log(response)
                }
            });
        },5000)
    })

    // $('#response').on('click',function(){
    // })

    $(`#stop`).on('click',function(){
        clearInterval(timeId)
        console.log("stop")
        
    })

    $(`#switch`).on('click',function(){
        send("switchStatus")
    })

    $(`#motor`).on('click',function(){
        send("motorStatus")
    })

    $(`#go_room`).on('click',function(){
        send("goRoom")
    })

    $(`#1`).on('click',function(){
        a="1"
    })
    $(`#0`).on('click',function(){
        a="0"
    })
})
