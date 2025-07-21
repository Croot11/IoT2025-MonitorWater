<nav class="navbar navbar-fixed-top">
    <div class="container-fluid d-flex flex-nowrap">
        <div class="navbar-btn">
            <button type="button" class="btn-toggle-offcanvas"><i class="lnr lnr-menu fa fa-bars"></i></button>
        </div>

        <div class="navbar-brand">
            <a href="{{ route('user.index') }}" style="text-decoration: none; color: inherit;" class="img-responsive logo">
                <span class="logo-symbol">Z</span>
                <span class="brand-name">NavyLee</span>
            </a>

        </div>
        <div class="navbar-right">
            <form id="navbar-search" class="navbar-form search-form">
                <input id="search-input" class="form-control" placeholder="Search here..." type="text">
                <button type="button" id="search-button" class="btn btn-default"><i class="icon-magnifier"></i></button>
            </form>

            <div id="navbar-menu">
                <ul class="nav navbar-nav">

                    <li>
                        <a href="page-login.html" class="icon-menu"><i class="icon-login"></i></a>
                    </li>
                </ul>
            </div>
        </div>


    </div>
</nav>
<style>
    .logo-symbol {
        font-family: "Brush Script MT", cursive;
        font-size: 28px;
        font-weight: bold;
        color: #000080;

        padding: 1px 6px;
        border-radius: 1px;
        margin-right: 5px;
    }

    .brand-name {
        color: white;
        font-family: "Times New Roman", Times, serif;
        font-weight: bold;
    }
</style>
<script>
    document.addEventListener("DOMContentLoaded", function() {
        document.getElementById('search-input').addEventListener('keydown', function (event) {
            if (event.key === 'Enter') {
                event.preventDefault();
                scrollToElement();
                clearInputFields();
            }
        });

        document.getElementById('search-button').addEventListener('click', function() {
            scrollToElement();
            clearInputFields(); // Xóa các input khi nhấn nút
        });
    });

    function scrollToElement() {
        var searchValue = document.getElementById('search-input').value.trim().toLowerCase();
        if (searchValue === "tank 1") {
            var targetElement = document.getElementById("water-tank-1");
            if (targetElement) {
                targetElement.scrollIntoView({ behavior: 'smooth', block: 'center' });
            } else {
                alert('Không tìm thấy nội dung phù hợp!');
            }
        }
        if (searchValue === "tank main") {
            var targetElement = document.getElementById("water-tank-main");
            if (targetElement) {
                targetElement.scrollIntoView({ behavior: 'smooth', block: 'center' });
            } else {
                alert('Không tìm thấy nội dung phù hợp!');
            }
        } else{
            alert('Không tìm thấy nội dung phù hợp!')
        }
    }
    function clearInputFields() {
        var inputFields = document.querySelectorAll('input');
        inputFields.forEach(function(input) {
            input.value = '';
        });
    }
</script>
