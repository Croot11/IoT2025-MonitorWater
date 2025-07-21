<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;

class Monitor extends Model
{
    protected $fillable = ['device_id', 'distance_to_water', 'water_quality', 'pump_status'];
}
