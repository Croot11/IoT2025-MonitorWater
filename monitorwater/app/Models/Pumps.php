<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Pumps extends Model
{
    protected $table='pumps';
    protected $fillable = ['id','device_id','status'];
    public $timestamps = false;
}
