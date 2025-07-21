<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     */
    public function up(): void
    {
        Schema::create('infor_tank', function (Blueprint $table) {
            $table->id();
            $table->Integer('device_id');
            $table->foreignId('device_id')->constrained('devices')->onDelete('cascade');
            $table->integer('depth');
            $table->integer('capacity');
            $table->integer('safe_level')->unsigned();

            $table->timestamps();

            $table->foreign('device_id')->references('id')->on('devices')->onDelete('cascade');
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('infor_tank');
    }
};
