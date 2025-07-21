<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use App\Events\WaterQualityUpdate;

class Giamsatbe extends Model
{
    protected $table = 'water_quality_log';
    public $timestamps = false;
    protected $fillable = ['device_id', 'water_level', 'water_quality'];

    protected static function booted(){
        static::created(function ($log){
            event(new WaterQualityUpdate($log));
        });
    }
}
