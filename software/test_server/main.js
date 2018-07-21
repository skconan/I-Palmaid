// let text=""
// $(function(){
//     setInterval(function(){
//         $.ajax({
//             type: "GET",
//             url: "http://exceed.srakrn.me/api/jacky-group/view",
//             dataType: "text",
//             success: function (response) {
//                 console.log("recieve")   
                
//                 if (text != response){
//                     $('#box').append(`
//                     <h3> Server: </h3>
//                     <div class="alert alert-success" role="alert" align="left">
//                     <p>${response}</p>
//                     </div><hr>
//                     `)
//                 }
//             },
//             fail: function(response){
//                 console.log(response) 
//             }
//         });
//     },3000)

//     $('#send').on('click',function(){
//         let message = $('#text-input').val()
//         if (message != ""){
//             $.ajax({
//                 type: "POST",
//                 url: "http://exceed.srakrn.me/api/jacky-group/set",
//                 data: {
//                     value: message
//                 },
//                 dataType: "text",
//                 success: function (data) {
//                     console.log(data)
//                     $('#box').append(`
//                     <h4 align="right"> Me: </h4>
//                     <div class="alert alert-primary" role="alert" align="right">
//                     <p>${message}</p>
//                     </div><hr>
//                     `)
//                     text = message
//                     $('#text-input').val('')
//                 }
//             });
//         }
//         else{
//             alert("error")
//         }

//        })
     
// })
$(function(){
setInterval(function(){
        let recieve = ""
        $.ajax({
            type: "GET",
            url: "http://ecourse.cpe.ku.ac.th:1515/api/iPalm-switchStatus/view",
            dataType: "text",
            success: function (response) {
                console.log("recieve")   
                console.log(response)
                recieve = response 
                // if (recieve = "1"){
                //     console.log("string")
                // }
                $.ajax({
                    type: "POST",
                    url: "http://ecourse.cpe.ku.ac.th:1515/api/iPalm-switchStatus/set",
                    data: {
                        value: Number(recieve)
                    },
                    dataType: "text",
                    success: function (response) {
                        console.log(response)
                        console.log("succes")
                        }
                    });        
            },
            fail: function(response){
                console.log(response)
            }
        });

        // if(recieve == "1"){
        // }
    },5000)
})
