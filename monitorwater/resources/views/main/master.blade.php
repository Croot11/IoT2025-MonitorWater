<!doctype html>
<html lang="en">


<!-- Mirrored from www.wrraptheme.com/templates/lucid/html/dark/chart-gauges.html by HTTrack Website Copier/3.x [XR&CO'2014], Thu, 13 Mar 2025 01:21:59 GMT -->
<head>
<title>Page Blank</title>
<meta charset="utf-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge, chrome=1">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0">
<meta name="description" content="Lucid Bootstrap 4.1.1 Admin Template">
<meta name="author" content="WrapTheme, design by: ThemeMakker.com">
<meta name="csrf-token" content="{{ csrf_token() }}">

<link rel="icon" href="favicon.ico" type="image/x-icon">
<!-- VENDOR CSS -->
<link rel="stylesheet" href="{{asset('assetss')}}/assets/vendor/bootstrap/css/bootstrap.min.css">
<link rel="stylesheet" href="{{asset('assetss')}}/assets/vendor/font-awesome/css/font-awesome.min.css">

<!-- MAIN CSS -->
<link rel="stylesheet" href="{{asset('assetss')}}/asset/css/main.css">
<link rel="stylesheet" href="{{asset('assetss')}}/asset/css/color_skins.css">
<link rel="stylesheet" href="{{asset('assetss')}}/myself/codeself.css">
</head>
<body class="theme-cyan">

    <!-- Page Loader -->

<!-- Overlay For Sidebars -->

<div id="wrapper">

    {{-- header --}}
    @include('main.layouts.header')

    {{-- menu --}}
    @include('main.layouts.menu')

    {{-- main content --}}
    @yield('main-content')

</div>
<script>
    function updatePumpStatus(id) {
        fetch(`/get-pump-status/${id}`)
            .then(response => response.json())
            .then(data => {
                let button = document.getElementById(`pump-button-${id}`);
                if (button) {
                    if (data.status == 1) {
                        button.innerText = "On";
                        button.classList.remove("btn-danger");
                        button.classList.add("btn-success");
                    } else {
                        button.innerText = "Off";
                        button.classList.remove("btn-success");
                        button.classList.add("btn-danger");
                    }
                }
            })
            .catch(error => console.error('Lỗi:', error));
    }

    function togglePump(id) {
        fetch(`/toggle-pump/${id}`, {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json',
                'X-CSRF-TOKEN': document.querySelector('meta[name="csrf-token"]').content
            },
            body: JSON.stringify({})
        })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                updatePumpStatus(id);
            } else {
                console.error("Lỗi cập nhật:", data.error);
            }
        })
        .catch(error => console.error('Lỗi gửi yêu cầu:', error));
    }

    // Cập nhật trạng thái của tất cả các bơm khi trang tải xong
    document.addEventListener("DOMContentLoaded", function() {
        let pumpIds = [1, 2]; // Danh sách ID máy bơm, thay đổi nếu cần

        function updateAllPumpStatus() {
            pumpIds.forEach(id => updatePumpStatus(id));
        }

        // Gọi ngay lần đầu khi trang tải xong
        updateAllPumpStatus();

        // Cập nhật trạng thái mỗi 10 giây (10,000ms)
        setInterval(updateAllPumpStatus, 100);
    });
</script>

<!-- Javascript -->
<script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>

<script src="{{asset('assetss')}}/asset/bundles/libscripts.bundle.js"></script>
<script src="{{asset('assetss')}}/asset/bundles/vendorscripts.bundle.js"></script>

<script src="{{asset('assetss')}}/assets/vendor/gauge/gauge.min.js"></script>
<script src="{{asset('assetss')}}/asset/bundles/chartist.bundle.js"></script>
<script src="{{asset('assetss')}}/asset/bundles/knob.bundle.js"></script>
<script src="{{asset('assetss')}}/assets/vendor/toastr/toastr.js"></script>

<script src="{{asset('assetss')}}/asset/bundles/mainscripts.bundle.js"></script>
<script src="{{asset('assetss')}}/asset/js/index.js"></script>
<script src="{{asset('assetss')}}/asset/js/script.js"></script>

<script src="{{asset('assetss')}}/asset/js/pages/chart/gauge.js"></script>

</body>

<!-- Mirrored from www.wrraptheme.com/templates/lucid/html/dark/chart-gauges.html by HTTrack Website Copier/3.x [XR&CO'2014], Thu, 13 Mar 2025 01:22:00 GMT -->
</html>
