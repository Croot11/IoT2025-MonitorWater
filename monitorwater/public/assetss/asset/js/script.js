

$(document).ready(function(){
    console.log("Script đã chạy"); // Kiểm tra xem script có chạy không

    $.ajax({
        url: "/main/api_namedevice",
        type: "GET",
        success: function(response){
            console.log("Dữ liệu API:", response); // Kiểm tra dữ liệu API

            let dropdown = $("#device_infor");
            dropdown.empty(); // Xóa danh sách cũ
            dropdown.append('<option value="">-- Chọn thiết bị --</option>');

            // Đảm bảo response là một mảng
            if (!Array.isArray(response)) {
                response = JSON.parse(response);
            }

            response.forEach(function(device) {
                console.log("Thêm thiết bị:", device.name); // Debug từng thiết bị
                dropdown.append(`<option value="${device.id}">${device.name}</option>`);
            });
        },
        error: function(xhr){
            console.log("Lỗi khi tải danh sách thiết bị: ", xhr.responseText);
        }
    });
});

$(document).ready(function(){
    $("#saveSettings").click(function(){
        let device_id = $("#device_infor").val();
        let depth = $("#depth").val();
        let capacity = $("#volume").val();
        let safe_level = $("#threshold").val();

        if(!device_id){
            alert("vui long chon thiet bi!");
            return;
        }
        $.ajax({
            url: "/main/api_updatetank/"+device_id,
            type: "PUT",
            contentType:"application/json",
            data: JSON.stringify({
                device_id: device_id,
                depth: depth,
                capacity: capacity,
                safe_level: safe_level
            }),
            success: function(response){
                alert("success");
                $("#settingModal").modal("hide");
                $("#settingsForm")[0].reset();
            },
            error: function(xhr){
                alert("faild");
                console.log(xhr.responseText);
            }
        });

    })
})
// $(document).ready(function(){
//     $("#saveSettings").click(function(){
//         // $('#settingsModal').on('shown.bs.modal', function () {
//         //     $(this).removeAttr('aria-hidden');
//         // });

//         // $('#settingsModal').on('hidden.bs.modal', function () {
//         //     $(this).attr('aria-hidden', 'true');
//         // });
//         let device_id = $("#device_infor").val();
//         let depth = $("#depth").val();
//         let capacity = $("#volume").val();
//         let safe_level = $("#threshold").val();

//         $.ajax({
//             url:"/main/api_tank",
//             type:"POST",
//             data: {
//                 device_id: device_id,
//                 depth: depth,
//                 capacity: capacity,
//                 safe_level: safe_level,

//             },
//             success: function (response){
//                 alert("success!");
//                 $("#settingsModal").modal("hide");
//             },
//             error: function(xhr){
//                 alert("faild");
//                 console.log(xhr.reponseText);
//             }

//         })
//     })
// })
