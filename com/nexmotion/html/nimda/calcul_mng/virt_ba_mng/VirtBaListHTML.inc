<?
/*
 * 검색 list 생성
 *
 * @param $result->fields["manu_name"] = "외부업체 이름"
 * @param $result->fields["extnl_etprs_seqno"] = "외부업체 일련번호"
 *
 * return : list
 */
function makeSearchCndList($result, $func) {

    $buff = "";


    while ($result && !$result->EOF) {

        if ($func == "ba") {

            $data = $result->fields["ba_num"]; 
            $data_val = $result->fields["ba_num"]; 

        } else {

            $data = $result->fields["member_name"]; 
            $data_val = $result->fields["member_seqno"]; 

        }

        $opt_arr[$data] = $data_val; 
        $result->moveNext();
    }

    //옵션 값을 셋팅
    if (is_array($opt_arr)) {

        foreach($opt_arr as $key => $val) {

            $buff .= "<a href=\"#\" onclick=\"" . $func;
            $buff .= "Click('" . $val . "', '" . $key;
            $buff .= "')\"><li>" . $key . "</li></a>";

        }
    }

    return $buff;
}

/* 
 * 가상계좌 list  생성 
 * $result : $result->fields["ba_num"] = "계좌번호" 
 * $result : $result->fields["state"] = "상태" 
 * $result : $result->fields["member_seqno"] = "회원일련번호" 
 * $result : $result->fields["member_name"] = "회원명" 
 * $result : $result->fields["bank_name"] = "은행이름" 
 * $result : $result->fields["ba_seqno"] = "계좌 일련번호" 
 * 
 * return : list
 */
function makeVirtBaList($result, $list_count) {

    $ret = "";
    $i = 1 * ($list_count+1);

    while ($result && !$result->EOF) {

        $ba_num = $result->fields["ba_num"];
        $state = $result->fields["state"];
        $bank_name = $result->fields["bank_name"];
        $member_seqno = $result->fields["member_seqno"];
        $member_name = $result->fields["member_name"];
        $ba_seqno = $result->fields["virt_ba_admin_seqno"];
        $cpn_admin_seqno = $result->fields["cpn_admin_seqno"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n    <td>%d</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td><button onclick=\"loadVirtBaDetail";
        $list .= "(%d, %d, '%s','%s', '%s', '%s')\" class=\"bgreen btn_pu btn";
        $list .= " fix_height20 fix_width40\">수정</button>";
        $list .= " <button onclick=\"removeMemberVirtBa(%d)\"";
        $list .= " class=\"bgreen btn_pu btn fix_height20 fix_width40\">";
        $list .= "삭제</button></td>";
        $list .= "\n  </tr>";

        $ret .= sprintf($list, $i, $bank_name, $member_name, $ba_num,
                        $ba_seqno, $cpn_admin_seqno, $bank_name,
                        $member_seqno, $member_name, $ba_num, $ba_seqno);

        $result->moveNext();
        $i++;
    }

    return $ret;
}

/*
 * 가상계좌 list  생성
 * $result : $result->fields["ba_num"] = "계좌번호"
 * $result : $result->fields["state"] = "상태"
 * $result : $result->fields["member_seqno"] = "회원일련번호"
 * $result : $result->fields["member_name"] = "회원명"
 * $result : $result->fields["bank_name"] = "은행이름"
 * $result : $result->fields["ba_seqno"] = "계좌 일련번호"
 *
 * return : list
 */
function makeVirtBaChangeList($result, $list_count) {

    $ret = "";
    $i = 1 * ($list_count+1);

    while ($result && !$result->EOF) {

        $depo_name = $result->fields["depo_name"];
        $bank_before = $result->fields["bank_before"];
        $bank_aft = $result->fields["bank_aft"];
        $state = $result->fields["prog_state"];
        $bank_name = $result->fields["bank_name"];
        $member_seqno = $result->fields["member_seqno"];
        $member_name = $result->fields["member_name"];
        $virt_ba_change_history_seqno = $result->fields["virt_ba_change_history_seqno"];
        $cpn_admin_seqno = $result->fields["cpn_admin_seqno"];

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n    <td>%d</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td><button onclick=\"loadVirtBaChangeDetail";
        $list .= "(%d)\" class=\"bgreen btn_pu btn";
        $list .= " fix_height20 fix_width40\">관리</button>";
        $list .= "</td>";
        $list .= "\n  </tr>";

        $ret .= sprintf($list, $i, $member_name,$depo_name, $bank_before, $bank_aft, $state,
            $virt_ba_change_history_seqno);

        $result->moveNext();
        $i++;
    }

    return $ret;
}


function makeVirtBaRefundList($result, $list_count) {

    $ret = "";
    $i = 1 * ($list_count+1);

    while ($result && !$result->EOF) {
        $refund_name = $result->fields["refund_name"];
        $member_name = $result->fields["member_name"];
        $req_date = $result->fields["req_date"];
        $req_price = $result->fields["req_price"];
        $member_refund_history_seqno = $result->fields["member_refund_history_seqno"];
        $refund_yn = $result->fields["refund_yn"];
        $refund_ba_num = $result->fields["refund_ba_num"];
        $refund_bank_name = $result->fields["refund_bank_name"];
        $display_none = "";
        $state = "신청중";
        if($refund_yn == "Y") {
            $state = "완료";
            $display_none = "style='display:none;'";
        }

        if ($i%2 == 1) {
            $list  = "\n  <tr>";
        } else {
            $list  = "\n  <tr class=\"cellbg\">";
        }
        $list .= "\n    <td>%d</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td>%s</td>";
        $list .= "\n    <td><button %s onclick=\"loadVirtBaRefundDetail";
        $list .= "(%d)\" class=\"bgreen btn_pu btn";
        $list .= " fix_height20 fix_width80\">환불완료</button>";
        $list .= "</td>";
        $list .= "\n  </tr>";

        $ret .= sprintf($list, $i, $member_name,$refund_name, $refund_bank_name, $refund_ba_num, number_format($req_price) . "원", $req_date, $state,
            $display_none, $member_refund_history_seqno);

        $result->moveNext();
        $i++;
    }

    return $ret;
}
?>
