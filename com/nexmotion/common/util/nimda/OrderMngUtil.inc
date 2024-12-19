<?php
/*
 * Copyright (c) 2017 Nexmotion, Inc.
 * All rights reserved.
 *
 * REVISION HISTORY (reverse chronological order)
 *=============================================================================
 * 2017/07/11 엄준현 생성
 * 2017/07/11 이청산 수정
 *=============================================================================
 */
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . '/common_lib/CommonUtil.inc');

class OrderMngUtil extends CommonUtil {

    /**
     * @brief 거래타입코드번호를 이용해 코드명 판별
     *
     * @param $input_typ = 거래타입코드번호
     *
     * @return 코드명
     */
    function selectDepoInputType($input_typ) {
        $typ_p = substr($input_typ, 0, 1);
        $typ_s = substr($input_typ, 1);
    
        $type_name = DEPO_INPUT_TYPE[$typ_p][$typ_s]; 

        return $type_name;
    }

    /**
     * @brief 생산한도설정 입금 코드를 이용해 코드명 판별
     *
     * @param $depo_yn = 생산한도 입금 코드번호
     *
     * @return 코드명
     */     
    function selectDepoYn($depo_yn) {

        $depo_status = DEPO_YN[$depo_yn];

        if ($depo_yn == "00") {
            $depo_ret  = "<td style=\"color:blue\">";
            $depo_ret .= $depo_status;
            $depo_ret .= "</td>";

        } else if ($depo_yn == "01") {
            $depo_ret  = "<td style=\"color:red\">";
            $depo_ret .= $depo_status;
            $depo_ret .= "</td>";

        }

        return $depo_ret;
    }
    
    /**
     * @brief 생산한도설정 입금 코드를 이용해 코드명 판별(td 없는 함수)
     * @comment 엑셀에서는 td를 굳이 씌워서 하이라이팅 할 필요가 없음
     *
     * @param $depo_yn = 생산한도 입금 코드번호
     *
     * @return 코드명
     */     
    function selectDepoYnForExcel($depo_yn) {

        $depo_status = DEPO_YN[$depo_yn];

        return $depo_status;
    }



    /**
     * @brief 생산한도 카테고리 코드를 이용해 코드명 판별
     *
     * @param $limit_cate = 생산한도 입금 코드번호
     *
     * @return 코드명
     */     
    function selectLimitCate($limit_cate) {

        $cate_name = LIMIT_CATE[$limit_cate];

        return $cate_name;
    }
}
?>
