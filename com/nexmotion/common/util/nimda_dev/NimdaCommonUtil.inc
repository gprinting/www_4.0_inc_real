<?php
include_once(INC_PATH . '/common_lib/CommonUtil.inc');

class NimdaCommonUtil extends CommonUtil {
    /**
     * @brief 회원등급정책 검색결과에서 회원 등급배열 생성
     *
     * @param $rs = 회원등급정책 검색결과
     *
     * @return 회원 등급배열
     */
    public function getMemberGradeArr($rs) {
        $ret = [];

        while ($rs && !$rs->EOF) {
            $flds = $rs->fields;

            $ret[$flds["grade"]] = $flds["erp_grade_name"];

            $rs->MoveNext();
        }

        return $ret;
    }
}
