<?php

namespace App\Listeners;

use App\Events\WaterQualityUpdate;
use Illuminate\Contracts\Queue\ShouldQueue;
use Illuminate\Queue\InteractsWithQueue;
use App\Models\Giamsatbe;
use App\Models\Pumps;
use App\Models\Infortank;

class UpdatePumpStatus
{
    /**
     * Create the event listener.
     */
    public function __construct()
    {
        //
    }

    /**
     * Handle the event.
     */
    public function handle(WaterQualityUpdate $event): void
    {
        $log = $event->log;
        $threshold = Infortank::where('device_id', $log->device_id)->value('safe_level');
        $status = Pumps::where('device_id', $log->device_id)->value('status');

        if($threshold){
            $status = ($log->water_level < 20) ? '1' : (($log->water_level > 95) ? '0' : $status);
            Pumps::where('device_id',$log->device_id)->update(['status'=>$status]);
        }
        \Log::info('Listener đã chạy!', ['data' => $event->log]);

    }
}
