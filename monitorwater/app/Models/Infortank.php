<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Infortank extends Model
{
    use HasFactory;
    protected $table = 'informationtank';
    protected $fillable = ['device_id','depth', 'capacity','safe_level'];
}
