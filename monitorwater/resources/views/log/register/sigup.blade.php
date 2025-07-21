@extends('log.master')
@section('title', 'Register')
@section('main-content')

<section class="content">
    <div class="card">
        <div class="header">
            <p class="lead">Create an account</p>
        </div>
        <div class="body">
            <form id="register-form" class="form-auth-small">
                @csrf
                <div class="form-group">
                    <label for="signup-email" class="control-label sr-only">Email</label>
                    <input type="email" class="form-control" id="signup-email" name="email" placeholder="Your email">
                </div>
                <div class="form-group">
                    <label for="signup-password" class="control-label sr-only">Password</label>
                    <input type="password" class="form-control" id="signup-password" name="password" placeholder="Password">
                </div>
                <button type="submit" class="btn btn-primary btn-lg btn-block">REGISTER</button>
                <div class="bottom">
                    <span class="helper-text">Already have an account? <a href="{{ route('log.index') }}">Login</a></span>
                </div>
                <p id="register-message"></p>
            </form>
        </div>
    </div>
</section>

<script>
document.getElementById('register-form').addEventListener('submit', function(event) {
    event.preventDefault();

    let email = document.getElementById('signup-email').value.trim();
    let password = document.getElementById('signup-password').value.trim();
    let message = document.getElementById('register-message');

    if (email === '' || password === '') {
        message.innerHTML = "<span style='color: red;'>Please fill in all fields.</span>";
        return;
    }

    fetch("/register", {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
            'X-CSRF-TOKEN': document.querySelector('input[name=_token]').value
        },
        body: JSON.stringify({ email, password })
    })
    .then(response => response.json())
    .then(data => {
        if (data.success) {
            message.innerHTML = "<span style='color: green;'>" + data.message + "</span>";
            setTimeout(() => window.location.href = "{{ route('log.index') }}", 2000);
        } else {
            message.innerHTML = "<span style='color: red;'>" + data.message + "</span>";
        }
    })
    .catch(error => console.error('Error:', error));
});
</script>

@endsection
