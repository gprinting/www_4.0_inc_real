<?
ob_start();    

//총게시물수 , 현재 페이지 , 블록당게시물수, js함수명, js함수 param
function mkDotAjaxPage($totalCnt , $page , $LIST_NUM , $func, $param=""){

	//총게시물 수가 1개 미만이면 빈값 리턴
	if ($totalCnt < 1) return "";
	
	$totalpage = @ceil($totalCnt / $LIST_NUM); //게시판 총 페이지수

	//페이지로직 처음 보여줄 페이지수
	//$first = $page - 10;
	$first = ((ceil($page/10)-1) * 10)+1;
	
	if ($first <= 1) $first = 1;
	
	//페이지로직 마지막 보여줄 페이지수
	$last = ceil($page /10) * 10;
	if ($last > $totalpage) $last = $totalpage;

	$prev = $first - 10;
	$nxt = $first + 10;

//  $ret  = "\n<ul class=\"paging\">";

    //처음 앞으로 이동
    if ($param == "") {
	    $prevurl = "onClick=\"" . $func . "(1)\"";
    } else {
	    $prevurl = "onClick=\"" . $func . "(1, '" . $param . "')\"";
    }

    if ($page == 1) {
        $ret .= "\n    <li class=\"first\"><button title=\"첫 페이지로\" disabled=\"disabled\">1</button></li>";
    } else {
        $ret .= "\n    <li class=\"first\"><button title=\"첫 페이지로\" $prevurl>1</button></li>";

    }

    //1블록 앞으로 이동
	if ($prev > 0) {
        if ($param == "") {
            $prevurl = "onClick=\"" . $func . "(" . $prev . ")\"";
        } else {
            $prevurl = "onClick=\"" . $func . "(" . $prev . ", '" . $param . "')\"";
        }
    } else {
        if ($param == "") {
            $prevurl = "onClick=\"" . $func . "(1)\"";
        } else {
            $prevurl = "onClick=\"" . $func . "(1, '" . $param . "')\"";
        }
    }

    if ($page == 1) 
        $ret .= "\n    <li class=\"prev\"><button title=\"이전 페이지로\" disabled=\"disabled\">←</button></li>";
    else 
        $ret .= "\n    <li class=\"prev\"><button title=\"이전 페이지로\" $prevurl>←</button></li>";
 
	for ($x = $first; $x <= $last; $x++) {
		if ($x == $page) {
            $ret .= "\n    <li><button class=\"on\" >$x</button></li>";
		} else {
            if ($param == "") {
                $ret .= "\n    <li><button onclick=\"" . $func  . "(" . $x . ");\">$x</button></li>";
            } else {
                $ret .= "\n    <li><button onclick=\"" . $func  . "(" . $x . ", '" . $param . "');\">$x</button></li>";
            }
		}
	}

    //1블록 뒤로 이동
	if ($nxt <= $totalpage) {
        if ($param == "") {
	        $nexturl = "onClick=\"" . $func . "(" . $nxt . ")\"";
        } else {
	        $nexturl = "onClick=\"" . $func . "(" . $nxt . ", '" . $param . "')\"";
        }
    } else {
        if ($param == "") {
            $nexturl = "onClick=\"" . $func . "(" . $totalpage . ")\"";
        } else {
            $nexturl = "onClick=\"" . $func . "(" . $totalpage . ", '" . $param . "')\"";
        }
    }

    if ($nxt+1 == $totalpage)
        $ret .= "\n    <li class=\"next\"><button title=\"다음 페이지로\" $nexturl>→</button></li>";
    //10페이지 이하일때도 마지막 페이지로 갈수 있도록 하기 위해 수정
    //else if ($totalpage < 10)
        //$ret .= "\n    <li class=\"next\"><button title=\"다음 페이지로\" disabled=\"disabled\">→</button></li>";
    else if ($totalpage == $page)
        $ret .= "\n    <li class=\"next\"><button title=\"다음 페이지로\" disabled=\"disabled\">→</button></li>";
    else 
        $ret .= "\n    <li class=\"next\"><button title=\"다음 페이지로\" $nexturl>→</button></li>";

    //맨뒤로 이동
    if ($param == "") {
	    $nexturl = "onClick=\"" . $func . "(" . $totalpage . ")\"";
    } else {
	    $nexturl = "onClick=\"" . $func . "(" . $totalpage . ", '" . $param . "')\"";
    }

    if ($page == $totalpage)
        $ret .= "\n    <li class=\"last\"><button title=\"마지막 페이지로\" disabled=\"disabled\">$totalpage</button></li>";
    else 
    $ret .= "\n    <li class=\"last\"><button title=\"마지막 페이지로\" $nexturl>$totalpage</button></li>";

//  $ret .= "\n</ul>";

	return $ret;
}




// [모바일] 총게시물수 , 현재 페이지 , 블록당게시물수, js함수명, js함수 param
function mkDotAjaxPageMobile($totalCnt , $page , $LIST_NUM , $func, $param=""){

	//총게시물 수가 1개 미만이면 빈값 리턴
	if ($totalCnt < 1) return "";
	
	$totalpage = @ceil($totalCnt / $LIST_NUM); //게시판 총 페이지수

	//페이지로직 처음 보여줄 페이지수
	//$first = $page - 10;
	$first = ((ceil($page/3)-1) * 3)+1;
	
	if ($first <= 1) $first = 1;
	
	//페이지로직 마지막 보여줄 페이지수
	$last = ceil($page /3) * 3;
	if ($last > $totalpage) $last = $totalpage;

	$prev = $first - 1;
	$nxt = $first + 3;

//  $ret  = "\n<ul class=\"paging\">";

    //처음 앞으로 이동
    if ($param == "") {
	    $prevurl = "onClick=\"" . $func . "(1)\"";
    } else {
	    $prevurl = "onClick=\"" . $func . "(1, '" . $param . "')\"";
    }

    if ($page == 1) {
        $ret .= "\n    <span class=\"first\"><img src=\"/design_template/m/images/common/btn_board_first.png\" alt=\"처음으로\" />1</span>";
    } else {
        $ret .= "\n    <span class=\"first\"><img src=\"/design_template/m/images/common/btn_board_first.png\" alt=\"처음으로\" $prevurl>1</span>";

    }

    //1블록 앞으로 이동
	if ($prev > 1) {
        if ($param == "") {
            $prevurl = "onClick=\"" . $func . "(" . $prev . ")\"";
        } else {
            $prevurl = "onClick=\"" . $func . "(" . $prev . ", '" . $param . "')\"";
        }
    } else {
        if ($param == "") {
            $prevurl = "onClick=\"" . $func . "(1)\"";
        } else {
            $prevurl = "onClick=\"" . $func . "(1, '" . $param . "')\"";
        }
    }

    if ($page == 1) 
        $ret .= "\n    <span class=\"prev\"><img src=\"/design_template/m/images/common/btn_board_prev.png\" alt=\"이전페이지\"></span>";
    else 
        $ret .= "\n    <span class=\"prev\" $prevurl><img src=\"/design_template/m/images/common/btn_board_prev.png\" alt=\"이전페이지\"></span>";
 
	for ($x = $first; $x <= $last; $x++) {
		if ($x == $page) {
            $ret .= "\n    <span class=\"page_btn_select_page\">$x</span>";
		} else {
            if ($param == "") {
                $ret .= "\n    <span onclick=\"" . $func  . "(" . $x . ");\">$x</span>";
            } else {
                $ret .= "\n    <span onclick=\"" . $func  . "(" . $x . ", '" . $param . "');\">$x</span>";
            }
		}
	}

    //1블록 뒤로 이동
	if ($nxt <= $totalpage) {
        if ($param == "") {
	        $nexturl = "onClick=\"" . $func . "(" . $nxt . ")\"";
        } else {
	        $nexturl = "onClick=\"" . $func . "(" . $nxt . ", '" . $param . "')\"";
        }
    } else {
        if ($param == "") {
            $nexturl = "onClick=\"" . $func . "(" . $totalpage . ")\"";
        } else {
            $nexturl = "onClick=\"" . $func . "(" . $totalpage . ", '" . $param . "')\"";
        }
    }

    if ($nxt+1 == $totalpage)
        $ret .= "\n    <span class=\"next\" $nexturl><img src=\"/design_template/m/images/common/btn_board_next.png\" alt=\">\"></span>";
    //10페이지 이하일때도 마지막 페이지로 갈수 있도록 하기 위해 수정
    //else if ($totalpage < 10)
        //$ret .= "\n    <li class=\"next\"><button title=\"다음 페이지로\" disabled=\"disabled\">→</button></li>";
    else if ($totalpage == $page)
        $ret .= "\n    <span class=\"next\"><img src=\"/design_template/m/images/common/btn_board_next.png\" alt=\">\"></span>";
    else 
        $ret .= "\n    <span class=\"next\" $nexturl><img src=\"/design_template/m/images/common/btn_board_next.png\" alt=\">\"></span>";

    //맨뒤로 이동
    if ($param == "") {
	    $nexturl = "onClick=\"" . $func . "(" . $totalpage . ")\"";
    } else {
	    $nexturl = "onClick=\"" . $func . "(" . $totalpage . ", '" . $param . "')\"";
    }

    if ($page == $totalpage) {
        $ret .= "\n    <span class=\"last\"><img src=\"/design_template/m/images/common/btn_board_last.png\" />$totalpage</span>";
    } else { 
        $ret .= "\n    <span class=\"last\"><img src=\"/design_template/m/images/common/btn_board_last.png\" $nexturl>$totalpage</span>";
    }

//  $ret .= "\n</ul>";

	return $ret;
}

?>
