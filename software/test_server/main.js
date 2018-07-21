// var refreshIntervalId = setInterval(fname, 10000);
var recieve = ""
$(function(){
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
                },
                fail: function(response){
                    console.log(response)
                }
            });
        },5000)
    })
    $(`#stop`).on('click',function(){
        clearInterval(timeId)
        console.log("stop")
    //     clearInterval(refreshIntervalId);
    })

    $(`#send`).on('click',function(){
        let a = $('#text-input').val()
        $.ajax({
            type: "POST",
            url: "http://ecourse.cpe.ku.ac.th/exceed/api/iPalm-switchStatus/set",
            data: {
                value: Number(a)
            },
            dataType: "text",
            success: function (response) {
                console.log(response)
                console.log("success post " + a)
                }
            });
    })
})
