<div id="left-sidebar" class="sidebar">
    <div class="sidebar-scroll">
        <div class="user-account" style="display: flex; align-items: center;">
            <img src="{{asset('assetss')}}/assets/images/sicula.png" class="rounded-circle user-photo" alt="User Profile Picture" style="width: 50px; height: 50px; margin-right: 10px;">

            <div>
                <span>Welcome,</span><br>
                <strong>Dương Xuân Ngọc</strong>
            </div>
        </div>
        <hr>

        <!-- Tab panes -->
        <div class="tab-content p-l-0 p-r-0">
            <div class="tab-pane active" id="menu">
                <nav id="left-sidebar-nav" class="sidebar-nav">
                    <ul id="main-menu" class="metismenu">
                        <li class="active">
                            <a href="#Dashboard" class="has-arrow"><i class="icon-home"></i> <span>Dashboard</span></a>
                            <ul>
                                <li class="active"><a href="{{ route('user.index') }}">S1.01</a></li>
                                <li><a href="index2.html">S1.02</a></li>
                                <li><a href="index3.html">S1.03</a></li>
                                <li><a href="index4.html">S1.04</a></li>
                                <li><a href="index5.html">S1.05</a></li>

                            </ul>
                        </li>
                    </ul>
                </nav>
            </div>
        </div>
    </div>
</div>
