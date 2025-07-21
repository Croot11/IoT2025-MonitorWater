<?php

namespace App\Http\Controllers\main;

use App\Http\Controllers\Controller;
use Illuminate\Http\Request;
use App\Models\Giamsatbe;
use App\Models\Pumps;
use App\Models\Infortank;
use App\Models\Devices;

class DashBoardController extends Controller
{
    public function index(){
        $water_quality_log1 = Giamsatbe::where('device_id', 1)
        ->orderBy('recorded_at', 'desc')
        ->first();
        $water_quality_log2 = Giamsatbe::where('device_id', 2)
        ->orderBy('recorded_at', 'desc')
        ->first();
        //$pumps = Pumps::all();
        //dd($monitors);
        return view('main.index',compact('water_quality_log1','water_quality_log2'));
    }
    public function store(Request $request){
        try {
            $data = $request->validate([
                'device_id' => 'required|numeric',
                'water_level' => 'required|numeric',
                'water_quality' => 'required|numeric',
            ]);

            $water_log = Giamsatbe::create($data);

            return response()->json([
                'message' => 'Dữ liệu đã được lưu thành công!',
                'data' => $water_log
            ], 201);
        } catch (\Exception $e) {
            return response()->json([
                'error' => 'Lỗi s',
                'message' => $e->getMessage()
            ], 500);
        }
    }
    public function update(Request $request, $id){
        try {
            $data = $request->validate([
                'device_id'=>'required|numeric',
                'depth'=>'required|numeric',
                'capacity'=>'required|numeric',
                'safe_level'=>'required|numeric'
            ]);

            $infor_tank = Infortank::find($id);
            if(!$infor_tank){
                return response()->json([
                    'message'=>'tank does not exist'
                ],404);
            }
            $infor_tank->update($data);
            return response()->json([
                'message'=>'success',
            ], 200);
        } catch(\Exception $e){
            return response()->json([
                'message'=> $e->getMessage()
            ], 500);
        }
    }
    public function infortank(Request $request){
        try {
            $data = $request->validate([
                'device_id'=>'required|numeric',
                'depth'=>'required|numeric',
                'capacity'=>'required|numeric',
                'safe_level'=>'required|numeric'
            ]);
            $infor_tank = Infortank::create($data);
            return response()->json([
                'message'=>'success'
            ],201);
        } catch (\Exception $e){
            return response()->json([
                'message'=>$e->getMessage()
            ], 500);
        }
    }
    public function checkconnect(Request $request, $id){
        $device = Devices::find($id);
        if(!$device){
            return response()->json(['message'=>'device not found'], 404);
        }
        $request->validate([
            'status'=>'required',
        ]);
        $device->status = $request->status;
        $device->save();
        return response()->json([
            'message'=>'device status updated successfully',
            'device'=>$device
        ]);
    }
    public function device(){
        $devices = Devices::all();
        return response()->json($devices);
    }
    public function informationtank(){
        $infor = Infortank::all();
        return response()->json($infor);
    }
}
