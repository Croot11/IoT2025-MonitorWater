<?php

use Illuminate\Support\Facades\Route;
use App\Http\Controllers\main\DashBoardController;
use App\Http\Controllers\log\HomeController;
use App\Http\Controllers\WaterLevelController;
use App\Models\Giamsatbe;
use App\Models\Pumps;
use Illuminate\Http\Request;
/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider and all of them will
| be assigned to the "web" middleware group. Make something great!
|
*/

Route::get('/', function () {
    return view('welcome');
});
Route::prefix('main')->group(function () {

    Route::get('/', [DashBoardController::class, 'index'])->name('user.index');
    Route::get('/api_namedevice',[DashBoardController::class, 'device']);
    Route::post('/api', [DashBoardController::class, 'store']);
    Route::post('/api_tank', [DashBoardController::class, 'infortank']);
    Route::put('/api_updatetank/{id}', [DashBoardController::class, 'update']);
    Route::put('/api_checkconnect/{id}', [DashBoardController::class, 'checkconnect']);
    Route::get('/api_infortank', [DashBoardController::class, 'informationtank']);
});
Route::prefix('log')->group(function () {
    Route::get('/', [HomeController::class, 'index'])->name('log.index');
    Route::get('/register', [HomeController::class, 'sigup'])->name('log.register.sigup');
});
Route::get('/get-distance-to-water', function () {
    $devices = [1, 2]; // Danh sách thiết bị
    $water_quality_logs = Giamsatbe::whereIn('device_id', $devices)
        ->orderBy('recorded_at', 'desc')
        ->get()
        ->groupBy('device_id') // Nhóm dữ liệu theo từng thiết bị
        ->map(fn ($logs) => $logs->first()); // Chỉ lấy bản ghi mới nhất của từng thiết bị

    if ($water_quality_logs->isEmpty()) {
        return response()->json([
            'message' => 'No data found for the requested devices'
        ], 404);
    }

    return response()->json([
        'device_1' => [
            'distance_to_water' => $water_quality_logs[1]->water_level ?? null,
            'water_quality' => $water_quality_logs[1]->water_quality ?? null,
        ],
        'device_2' => [
            'distance_to_water' => $water_quality_logs[2]->water_level ?? null,
            'water_quality' => $water_quality_logs[2]->water_quality ?? null,
        ],
    ]);
});
Route::get('/get-pump-status/{id}', function ($id) {
    $pump = Pumps::find($id);

    if (!$pump) {
        return response()->json(['status' => 0]); // Mặc định là OFF nếu không tìm thấy
    }

    return response()->json(['status' => $pump->status]);
});

Route::put('/toggle-pump/{id}', function ($id, Request $request) {
    try {
        \Log::info('PUT Request nhận được:', ['id' => $id]);

        $pump = Pumps::find($id);

        if (!$pump) {
            return response()->json(['success' => false, 'error' => 'Pump not found'], 404);
        }

        // Đảo trạng thái và lưu vào database
        $pump->status = !$pump->status;
        $pump->save();

        return response()->json(['success' => true, 'status' => $pump->status]);
    } catch (\Exception $e) {
        return response()->json(['success' => false, 'error' => $e->getMessage()], 500);
    }
});

Route::get('/register', [HomeController::class, 'showRegisterForm'])->name('register.form');
Route::post('/register', [HomeController::class, 'register'])->name('register');
