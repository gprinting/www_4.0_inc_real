<?
class LeftMenuSetting {

    function __construct() {
    }

    /**
      * @brief 레프트 메뉴 생성
      *
      * @param $left_arr : left define array
      * @param $menu_class_arr : left class define array
      * 
      */
    function getLeftMenuSetting ($left_arr, $top, $menu_class_arr, $visible_arr) {

        //차후 세션 개발후
        //if (!$sid) return false;
 
        $html  = "\n        <!-- LEFT NAVIGATION -->";
        $html .= "\n        <nav id=\"mainnav-container\">";
        $html .= "\n            <div id=\"mainnav-menu\" class=\"mainnav_menu\" style=\"clear:both;\">";
        $html .= "\n                   <dl class=\"list-group list-divider\">";

        foreach ($left_arr["sub"] as $key=>$value) {
            $is_visible = false;
            foreach($visible_arr as $k => $v) {
                if($left_arr[$key][$v] != null) {
                    $is_visible = true;
                }
            }

            if($is_visible) {
                $html .= "\n                    <dt class=\"mid " . $key . "\" onclick=\"midCheckLeft(this);\"><p><i class=\"" . $menu_class_arr[$key] . "\"></i> <span>" . $value . " </span></p></dt>";
                $html .= $this->getSubMenu($left_arr[$key], $top, $key, $visible_arr);
            }
        }
        $html .= "\n                        <dt class=\"list-divider\"></dt>";
        $html .= "\n                   </dl>";
        $html .= "\n            </div>";
        $html .= "\n        </nav>";

        return $html;
    }

    // $sid : 세션
    function loginInfo($sess) {
        $html = <<<HTML
            <div class="login_div">
                <div class="">
                    <h3 class="name"><span class="login_name_accent">{$sess["name"]}</span> 님</h3>
                    <button class="btn_admin_logout" type="button" onclick="location.replace('/common/logout.php');">LOGOUT</button>
                </div>;
                <span class="log">로그인시각</span>
                <span class="date">{$sess["login_date"]}</span>
            </div>
HTML;

        return $html;
    }

    //왼쪽 2Depth 메뉴 생성
    function getSubMenu($sub_arr, $top, $key, $arr_visible) {

        $html .= "\n                        <dd class=\"" . $key . "\">";
        $html .= "\n                             <ul>";

        foreach ($sub_arr as $key=>$value) {
            $tmp_key = $key;
            $kind = "";
            if($this->startsWith($key,"after_list")) {
                $tmp_key = "after_list";
                $kind = "?kind=" . $value;
            }
            if($this->startsWith($key,"basic_after_list")) {
                $tmp_key = "basic_after_list";
                $kind = "?kind=" . $value;
            }
            if(in_array($key ,$arr_visible))
                $html .= "\n                                 <li>";
            else
                $html .= "\n                                 <li style=''>";
            $html .= "\n                                 <a class=\"" . $key . "\" href=\"/" . $top . "/" . $tmp_key . ".html". $kind ."\">" . $value . " </a></li>";
        }
        
        $html .= "\n                             </ul>";
        $html .= "\n                        </dd>";

        return $html;
    }

    function startsWith($haystack, $needle) {
        // search backwards starting from haystack length characters from the end
        return $needle === "" || strrpos($haystack, $needle, -strlen($haystack)) !== false;
    }
}
?>
