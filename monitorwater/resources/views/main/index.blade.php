@extends('main.master')
@section('title', 'Trang chủ')
@section('main-content')
<section class="content">
    <div id="main-content">
        <div class="container-fluid">
            <div class="block-header">
                <div class="row">
                    <div class="col-lg-5 col-md-8 col-sm-12">
                        <h2><a href="javascript:void(0);" class="btn btn-xs btn-link btn-toggle-fullwidth"><i class="fa fa-arrow-left"></i></a> Dashboard</h2>
                        <ul class="breadcrumb">
                            <li class="breadcrumb-item"><a href="index.html"><i class="icon-home"></i></a></li>
                            <li class="breadcrumb-item active">Dashboard</li>
                        </ul>
                    </div>
                    <div class="col-lg-7 col-md-4 col-sm-12 text-right">
                        <h2> S1.01</h2>
                    </div>
                </div>
            </div>
            <div class="row clearfix">
                <div class="col-lg-5 col-md-12">
                    <div class="card">
                        <div class="header">
                            <h2 id="water-tank-main">MAIN WATER TANK</h2>
                            <ul class="header-dropdown">
                                <!-- Dropdown Settings -->
                                <li class="dropdown">
                                    <a href="javascript:void(0);" class="nav-link" data-toggle="modal" data-target="#settingsModal">
                                        <i class="icon-equalizer"></i>
                                    </a>
                                </li>

                                <!-- Modal Settings -->
                                <div class="modal fade" id="settingsModal" tabindex="-1" role="dialog" aria-labelledby="settingsModalLabel" aria-hidden="true">
                                    <div class="modal-dialog" role="document">
                                        <div class="modal-content">
                                            <div class="modal-header">
                                                <h5 class="modal-title" id="settingsModalLabel">Cài đặt bể nước</h5>
                                                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                                                    <span aria-hidden="true">&times;</span>
                                                </button>
                                            </div>
                                            <div class="modal-body">
                                                <form id="settingsForm">
                                                    @csrf
                                                    <div class="form-group">
                                                        <label for="device_infor">chọn thiết bị</label>
                                                        <select class="form-control" id="device_infor">
                                                            <option value="">--chọn thiết bị--</option>
                                                        </select>
                                                    </div>
                                                    <div class="form-group">
                                                        <label for="depth">Độ sâu của bể (m)</label>
                                                        <input type="number" class="form-control" id="depth" placeholder="Nhập độ sâu">
                                                    </div>
                                                    <div class="form-group">
                                                        <label for="volume">Thể tích bể nước (L)</label>
                                                        <input type="number" class="form-control" id="volume" placeholder="Nhập thể tích">
                                                    </div>
                                                    <div class="form-group">
                                                        <label for="threshold">Ngưỡng an toàn (%)</label>
                                                        <input type="number" class="form-control" id="threshold" placeholder="Nhập ngưỡng an toàn">
                                                    </div>
                                                    <div class="form-group">
                                                        <label for="mode">Chế độ</label>
                                                        <select class="form-control" id="mode">
                                                            <option value="auto">Auto</option>
                                                            <option value="manual">Manual</option>
                                                        </select>
                                                    </div>
                                                </form>
                                            </div>
                                            <div class="modal-footer">
                                                <button type="button" class="btn btn-secondary" data-dismiss="modal">Đóng</button>
                                                <button type="button" class="btn btn-primary" id="saveSettings">Lưu</button>
                                            </div>
                                        </div>
                                    </div>
                                </div>


                            </ul>
                        </div>

                            <div class="card">
                                <div class="body text-center">
                                    <input type="text" class="knob" data-linecap="round" value={{ $water_quality_log1->water_level }} data-width="250" data-height="250" data-thickness="0.25"data-fgColor="#64c8c0"data-readOnly="true">
                                    <p class="text-muted m-b-0">AMOUNT OF WATER REMAINING</p>
                                </div>
                            </div>

                    </div>
                </div>
                <div class="col-lg-3 col-sm-12">
                    <div class="card">
                        <div class="header">
                            <h2>Quality water(TDS)</h2>

                            <div class="body text-center">
                                <h2 id="zone-textfield" class="preview-textfield"></h2>
                                <canvas id="gauge-zone" width="150" height="150"></canvas>

                                <div class="gauge-labels">
                                    <span style="display: block; margin-bottom: 5px;">0 - 300ppm:  </b> Good</span>
                                    <span style="display: block; margin-bottom: 5px;">300 - 500ppm:  </b> Normal</span>
                                    <span style="display: block;">500 - 1000ppm:  </b> Bad</span>
                                </div>

                                </tbody>
                            </div>
                        </div>
                    </div>

                </div>
                <div class="col-lg-4 col-sm-12">
                    <div class="card">
                        <div class="header">
                            <h2>Information</h2>
                            <table class="table table-striped m-b-0">
                                <tbody>
                                    <tr>
                                    <td>Amount of water</td>
                                    <td class="font-medium">1000m3</td>
                                </tr>
                                <tr>
                                    <td>TDS</td>
                                    <td class="font-medium">300ppm</td>
                                </tr>
                                <tr>
                                    <td>Status connect</td>
                                    <td class="font-medium">On</td>
                                </tr>
                                <tr>
                                    <td>Pump status</td>
                                    <td>
                                        <button id="pump-button-1" class="btn btn-danger" onclick="togglePump(1)">
                                            Loading...
                                        </button>
                                    </td>
                                </tr>
                                </tbody>
                            </table>

                        </div>
                    </div>
                </div>
            </div>
            <div class="row clearfix">
                <div class="col-lg-5 col-md-12">
                    <div class="card">
                        <div class="header">
                            <h2 id="water-tank-1">WARTER TANK NUMBER 1</h2>

                        </div>

                            <div class="card">
                                <div class="body text-center">
                                    <input type="text" class="knob" data-linecap="round" value={{ $water_quality_log2->water_level }} data-width="250" data-height="250" data-thickness="0.25"data-fgColor="#64c8c0"data-readOnly="true">
                                    <p class="text-muted m-b-0">AMOUNT OF WATER REMAINING</p>
                                </div>
                            </div>

                    </div>
                </div>

                <div class="col-lg-4 col-sm-12">
                    <div class="card">
                        <div class="header">
                            <h2>Information</h2>
                            <table class="table table-striped m-b-0">
                                <tbody>
                                    <tr>
                                    <td>Amount of water</td>
                                    <td class="font-medium">1000m3</td>
                                </tr>
                                <tr>
                                    <td>Status connect</td>
                                    <td class="font-medium">On</td>
                                </tr>
                                <tr>
                                    <td>Pump status</td>
                                    <td>
                                        <button id="pump-button-2" class="btn btn-danger" onclick="togglePump(2)">
                                            Loading...
                                        </button>
                                        {{-- <button class="btn btn-danger" onclick="togglePump(this)">
                                            Off
                                        </button> --}}
                                    </td>
                                </tr>
                                </tbody>
                            </table>

                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <script>
        function updateDistanceToWater() {
            fetch('/get-distance-to-water')
                .then(response => response.json())
                .then(data => {
                    let knobs = document.querySelectorAll('.knob'); // Lấy tất cả knob
                    //let depth = depthElement ? parseFloat(depthElement.value) : 1;                    let scaledepth = 100/depth;
                    if (data.device_1 && knobs.length > 0) {
                        $(knobs[0]).val(data.device_1.distance_to_water).trigger('change');
                    }

                    if (data.device_2 && knobs.length > 1) {
                        $(knobs[1]).val(data.device_2.distance_to_water).trigger('change');
                    }
                })
                .catch(error => console.error('Error fetching data:', error));
        }

        setInterval(updateDistanceToWater, 1000); // Cập nhật mỗi giây
    </script>



</section>
@endsection
