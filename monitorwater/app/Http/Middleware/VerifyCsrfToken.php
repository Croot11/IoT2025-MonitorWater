<?php

namespace App\Http\Middleware;

use Illuminate\Foundation\Http\Middleware\VerifyCsrfToken as Middleware;

class VerifyCsrfToken extends Middleware
{
    /**
     * The URIs that should be excluded from CSRF verification.
     *
     * @var array<int, string>
     */
    protected $except = [
        'main/api',
        'main/api_tank',
        'main/api_updatetank/*',
        'main/api_updatetank',
        'main/api_checkconnect/*',
        'toggle-pump/*',
    ];
}
