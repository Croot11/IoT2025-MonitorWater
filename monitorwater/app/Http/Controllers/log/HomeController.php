<?php

namespace App\Http\Controllers\log;

use App\Http\Controllers\Controller;
use Illuminate\Http\Request;
use App\Models\Account;

class HomeController extends Controller
{
    public function index(){

        return view('log.index');
    }
    public function sigup(){
        return view('log.register.sigup');
    }
    public function register(Request $request)
    {
        // Kiểm tra nếu có email và password
        $request->validate([
            'username' => 'required|email|unique:users,email',
            'password' => 'required|min:6'
        ]);

        // Tạo tài khoản mới
        $user = new User();
        $user->email = $request->input('username');
        $user->password = Hash::make($request->input('password')); // Mã hóa mật khẩu
        $user->save();

        return response()->json(['success' => true, 'message' => 'Account registered successfully!']);
    }
    public function showRegisterForm()
    {
        return view('log.register.sigup');
    }
}
