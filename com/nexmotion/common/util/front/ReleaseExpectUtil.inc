<?php
/*
 * Copyright (c) Nexmotion, Inc.
 * All rights reserved.
 *
 * 출고예상일 계산
 *
 * @detail $deadline(마감)을 기준으로 $proc_time(공정), $after_time(후공정)이 가산됨
 * ex1) 코팅명함 당일판 12시 -> 12시 기준 + 공정 4시간 = 16시 출고
 * ex2) 코팅명함 후공정 박 -> 20시 기준 + 공정 16시간 = 익일 12시 출고
 *
 * @detail 파라미터 예시
 * $cate_sortcode = 001001001
 * $paper_info = 일반용지 아트지 백색 250g
 * $size_info = A4
 * $amt = 500
 * $page_arr = [4, 16, 16]
 * $after_info = 재단 [2등분]|오시 [1줄/세로]
 * $emergency = true
 *
 * REVISION HISTORY (reverse chronological order)
 *=============================================================================
 * 2018/02/22 엄준현 생성
 *=============================================================================
 */
define("INC_PATH", $_SERVER["INC"]);
include_once(INC_PATH . "/common_lib/DateUtil.inc");
include_once(INC_PATH . "/com/nexmotion/common/util/ConnectionPool.inc");
include_once(INC_PATH . "/common_dao/ProductCommonDAO.inc");

class ReleaseExpectUtil {
    private $conn;
    private $productDAO;
    private $dateUtil;

    private $sortcode_t; // string
    private $sortcode_m; // string
    private $sortcode_b; // string
    private $paper_info; // string
    private $size_info; // string
    private $amt; // string
    private $page_arr; // array
    private $after_info; // string
    private $emergency; // boolean

    private $expec_time; // array

    function __construct(array $param = []) {
        if (!empty($param)) {
            $this->setData($param);
        }

        $connectionPool = new ConnectionPool();
        $this->conn = $connectionPool->getPooledConnection();
        $this->productDAO = new ProductCommonDAO();
        $this->dateUtil = new DateUtil();
    }

    public function setData(array $param) {
        $cate_sortcode = $param["cate_sortcode"];
        $sortcode_t = substr($cate_sortcode, 0, 3);
        $sortcode_m = substr($cate_sortcode, 0, 6);

        $this->sortcode_t = $sortcode_t;
        $this->sortcode_m = $sortcode_m;
        $this->sortcode_b = $cate_sortcode;
        $this->paper_info = $param["paper_info"];
        $this->size_info  = $param["size_info"];
        $this->amt        = intval($param["amt"]);
        $this->page_arr   = $param["page_arr"];
        $this->after_info = $param["after_info"];
        $this->emergency  = $param["emergency"];
    }

    /**
     * @brief 예상출고일 계산
     *
     * @return [
     *     "month" => 월
     *     ,"day" => 일
     *     ,"hour" => 시
     * ]
     */
    public function calcReleaseExpectTime() {
        $ret = null;
        switch($this->sortcode_t) {
            case "001" :
                // 상업인쇄
                $ret = $this->getAdExpect();
                break;
            case "002" :
                // 실사출력
                $ret = $this->getAoExpect();
                break;
            case "003" :
                // 명함
                $ret = $this->getNcExpect();
                break;
            case "004" :
                // 스티커
                $ret = $this->getStExpect();
                break;
            case "005" :
                // 전단
                $ret = $this->getBlExpect();
                break;
            case "006" :
                // 봉투
                $ret = $this->getEvExpect();
                break;
            case "007" :
                // 경인쇄
                $ret = $this->getMtExpect();
                break;
            case "008" :
                // 기타인쇄
                $ret = $this->getEtcExpect();
                break;
            case "009" :
                // 그린백
                $ret = $this->getGbExpect();
                break;
            case "012" :
                // 그린백
                $ret = $this->getGbExpect();
                break;
        }

        return $ret;
    }

    /**
     * @brief 상업인쇄 예상출고일 계산
     *
     * @return [
     *     "month" => 월
     *     ,"day" => 일
     *     ,"hour" => 시
     * ]
     */
    private function getAdExpect() {
        // 포스터는 전단하고 동일취급
        if ($this->sortcode_b === "001003001") {
            return $this->getBlExpect();
        }

        $deadline = 19;
        $proc_time = 0;
        $after_time = 0;
        // 수량계수, 해당 값 * 후공정 시간
        $amt_coefficient = 1;

        $hour = date('G');
        $min  = date('i');

        $hm = intval($hour . $min);

        if ($hm > 1900) {
            $proc_time = 24;
        }

        if ($this->sortcode_b === "001001001") {
            if ($this->amt > 3000) {
                $proc_time += 24;
            }

            $sum_page = 0;
            foreach ($this->page_arr as $page) {
                $sum_page += intval($page);
            }
            if ($sum_page > 32) {
                $proc_time += 24;
            }
        }

        $after_info = explode('|', $this->after_info);

        foreach ($after_info as $after) {
            $aft_tmp = explode(' ', $after);
            if (strpos($aft_tmp[1], "중철제본") !== false) {
                $after_time += 48;
            } else if (strpos($aft_tmp[1], "무선제본") !== false) {
                $after_time += 72;
            }
        }

        return $this->calcExpect($deadline, $proc_time, $after_time);
    }

    /**
     * @brief 상업인쇄 예상출고일 계산
     *
     * @return [
     *     "month" => 월
     *     ,"day" => 일
     *     ,"hour" => 시
     * ]
     */
    private function getAoExpect() {
        $hour = date('G');
        $min  = date('i');

        $hm = intval($hour . $min);

        // 배너거치대는 주문 시간 +3시간
        // 19시 이후는 다음날 9시부터
        if ($this->sortcode_b === "002003001") {
            $cur_stamp = time();
            $temp = 3 * 3600;

            if ($hm > 1900) {
                $cur_stamp = mktime(9, 0, 0, date("n"), date("j") + 1, date("Y"));
                $temp = 0;
            }
            $expec_time = explode(' ', date("m d D H i", $cur_stamp + $temp));
            return [
                 "month"   => $expec_time[0]
                ,"day"     => $expec_time[1]
                ,"day_txt" => $expec_time[2]
                ,"hour"    => $expec_time[3]
                ,"min"     => $expec_time[4]
            ];
        }

        $deadline = 10;
        $proc_time = 0;
        $after_time = 0;

        $amt = $this->amt;

        if ($hm > 1000) {
            $proc_time = 24;
        }

        // 작업시간 추가
        if ($this->sortcode_b === "002001001") {
            // 미니배너
            $proc_time += 28;

            if ($amt >= 100) {
                $proc_time += 24;
            }
        } else if ($this->sortcode_b === "002005001"
                || $this->sortcode_b === "002005002") {
            // 게릴라 현수막, 족자 현수막
            $proc_time += 72;
        } else if ($this->sortcode_m === "002002"
                || $this->sortcode_b === "002005003") {
            // 배너, 현수막
            $proc_time += 28;

            if ($amt >= 20) {
                $proc_time += 24;
            }
        } else if ($this->sortcode_b === "002004001") {
            // 등신대
            $proc_time += 48;

            if ($amt >= 20) {
                $proc_time += 24;
            }
        }

        return $this->calcExpect($deadline, $proc_time, $after_time);
    }

    /**
     * @brief 명함 예상출고일 계산
     *
     * @return [
     *     "month" => 월
     *     ,"day" => 일
     *     ,"hour" => 시
     * ]
     */
    private function getNcExpect(): array {
        $deadline = 0;
        $proc_time = 0;
        $after_time = 0;

        $hour = date('G');
        $min  = date('i');

        $hm = intval($hour . $min);

        //!1 마감시간 설정
        if ($this->sortcode_m === "003003") {
            // 카드명함은 무조건 17시
            $deadline = 17;
        } else {
            // 일반, 수입명함은 당일판 여부에 따라서 다름
            if ($this->emergency) {
                if ($hm < 1200) {
                    $deadline = 12;
                } else if (1200 <= $hm && $hm < 1500) {
                    $deadline = 15;
                } else {
                    // 당일판 시간 이후 접수시간 내 당일판 체크시 익일 12시
                    $deadline = 12;
                    $proc_time = 24;
                }
            } else {
                $deadline = 20;
            }
        }

        // 당일 마감시 지나면 하루 추가
        if ($hm > 2000) {
            if ($this->emergency) {
                $deadline = 12;
            } else {
                $deadline = 20;
            }

            $proc_time = 24;
        }

        //!2 작업 소요시간 설정
        if ($this->sortcode_m === "003003") {
            // 카드명함은 원단별로 갈림
            $paper_name = trim(explode("카드", $this->paper_info)[0]);

            if (strpos("화이트|골드", $paper_name) !== false) {
                $proc_time += 48;
            } else if (strpos("실버|실버플러스|골드플러스|은펄|은펄플러스|금펄플러스|은펄반누드", $paper_name) !== false) {
                $proc_time += 72;
            } else {
                $proc_time += 96;
            }
        } else {
            // 일반, 수입명함은 당일판 여부에 따라서 다름
            if ($this->emergency) {
                $proc_time += 4;
            } else {
                $proc_time += 16;
            }


            // 수입명함 휘라레, 반누보, 스타드림 외에는 24시간 추가
            if ($this->sortcode_m === "003003") {
                $paper_name = trim(explode(' ', $this->paper_info)[0]);

                if (strpos("휘라레|반누보|스타드림", $paper_name) !== false) {
                    $proc_time += 24;
                }
            }
        }

        //!3 후가공 소요시간 설정
        if ($this->sortcode_m === "003003") {
            // 카드명함은 없음
            $after_time = 0;
        } else {
            if (!empty($this->after_info)) {
                $after_info = explode('|', $this->after_info);

                foreach ($after_info as $after) {
                    $aft_tmp = explode(' ', $after);
                    if (strpos("박|엠보싱|형압", $aft_tmp[0]) !== false) {
                        $after_time = 24;
                        break;
                    }
                }
            }
        }

        return $this->calcExpect($deadline, $proc_time, $after_time);
    }

    /**
     * @brief 스티커 예상출고일 계산
     *
     * @return [
     *     "month" => 월
     *     ,"day" => 일
     *     ,"hour" => 시
     * ]
     */
    private function getStExpect(): array {
        $deadline = 0;
        $proc_time = 0;
        $after_time = 0;

        $hour = date('G');
        $min  = date('i');

        $hm = intval($hour . $min);

        //!1 마감시간 설정
        // 당일판 여부에 따라서 다름
        if ($this->emergency) {
            if ($hm < 1200) {
                $deadline = 12;
            } else if (1200 <= $hm && $hm < 1500) {
                $deadline = 15;
            } else {
                // 당일판 시간 이후 접수시간 내 당일판 체크시 익일 12시
                $deadline = 12;
                $proc_time = 24;
            }
        } else {
            $deadline = 20;
        }

        // 당일 마감시 지나면 하루 추가
        if ($hm > 2000) {
            if ($this->emergency) {
                $deadline = 12;
            } else {
                $deadline = 20;
            }

            $proc_time = 24;
        }

        //!2 작업 소요시간 설정
        if ($this->sortcode_b === "004003009") {
            // 자유형 도무송
            $proc_time += 72;
        } else if ($this->sortcode_m === "004003") {
            // 도무송
            $proc_time += 24;
        } else {
            if ($this->emergency) {
                $proc_time += 4;
            } else {
                $proc_time += 16;
            }
        }

        //!3 후가공 소요시간 설정
        if (!empty($this->after_info)) {
            $after_info = explode('|', $this->after_info);

            foreach ($after_info as $after) {
                    $aft_tmp = explode(' ', $after);
                    if (strpos("박|엠보싱|형압", $aft_tmp[0]) !== false) {
                    $after_time = 24;
                    break;
                }
            }
        }

        return $this->calcExpect($deadline, $proc_time, $after_time);
    }

    /**
     * @brief 전단 예상출고일 계산
     *
     * @return [
     *     "month" => 월
     *     ,"day" => 일
     *     ,"hour" => 시
     * ]
     */
    private function getBlExpect(): array {
        $deadline = 0;
        $proc_time = 0;
        $after_time = 0;
        // 수량계수, 해당 값 * 후공정 시간
        $amt_coefficient = 0;

        $hour = date('G');
        $min  = date('i');

        $hm = intval($hour . $min);

        //!1 마감시간 설정
        if ($this->emergency) {
            if ($hm < 1200) {
                $deadline = 12;
            } else if (1200 <= $hm && $hm < 1500) {
                $deadline = 15;
            } else {
                // 당일판 시간 이후 접수시간 내 당일판 체크시 익일 12시
                $deadline = 12;
                $proc_time = 24;
            }
        } else {
            $deadline = 19;
        }

        // 당일 마감시 지나면 하루 추가
        if ($hm > 1900) {
            if ($this->emergency) {
                $deadline = 12;
            } else {
                $deadline = 19;
            }

            $proc_time = 24;
        }

        //!2 작업 소요시간 설정
        if ($this->emergency) {
            if ($hm < 1200) {
                $proc_time += 5;
            } else if (1200 <= $hm && $hm < 1500) {
                $proc_time += 4;
            }
        } else {
            $proc_time += 17;
        }

        //########## 비규격 사이즈일 때 시간

        //!2-1 독판전단 일 떄 수량별 소요시간 추가
        if ($this->cate_sortcode === "005002001") {
            if (strpos($this->size_info, 'A') !== false) {
                // 국전 사이즈 일 때 25R 이상시 24시간 추가
                if ($this->amt > 24) {
                    $amt_coefficient++;
                    $proc_time += 24;
                }
            } else if (strpos($this->size_info, 'B') !== false) {
                // 46 사이즈 일 때 13R 이상시 24시간 추가
                if ($this->amt > 12) {
                    $amt_coefficient++;
                    $proc_time += 24;
                }
            }
        }

        //!3 후가공 소요시간 설정
        if (!empty($this->after_info)) {
            $after_info = explode('|', $this->after_info);

            foreach ($after_info as $after) {
                $aft_tmp = explode(' ', $after);
                if (strpos("박|엠보싱|형압|넘버링|라미넥스|코팅", $aft_tmp[0]) !== false) {
                    $after_time = 24;
                    break;
                } else if ("접지" === $aft_tmp[0]
                        && strpos("4단|5단|6단", $aft_tmp[1]) !== false) {
                    $after_time = 24;
                    break;
                }
            }
        }

        return $this->calcExpect($deadline, $proc_time, $after_time);
    }

    /**
     * @brief 봉투 예상출고일 계산
     *
     * @return [
     *     "month" => 월
     *     ,"day" => 일
     *     ,"hour" => 시
     * ]
     */
    private function getEvExpect(): array {
        $deadline = 19;
        $proc_time = 0;
        $after_time = 0;

        $hour = date('G');
        $min  = date('i');

        $hm = intval($hour . $min);

        if ($hm > 1900) {
            $proc_time = 24;
        }

        //!2 작업 소요시간 설정
        if ($this->sortcode_b === "006001002"
                || strpos($this->size_info, "대봉투") !== false
                || strpos($this->size_info, "소봉투") !== false) {
            // 소봉투/자켓봉투/대봉투
            $proc_time += 20;
        } else {
            // 비규격/기타봉투
            $proc_time += 44;
        }

        //!3 후가공 소요시간 설정
        if (!empty($this->after_info)) {
            $after_info = explode('|', $this->after_info);

            foreach ($after_info as $after) {
                $aft_tmp = explode(' ', $after);
                if (strpos("접착", $aft_tmp[0]) !== false) {
                    $after_time = 48;
                    break;
                }
            }
        }

        return $this->calcExpect($deadline, $proc_time, $after_time);
    }

    /**
     * @brief 경인쇄 예상출고일 계산
     *
     * @return [
     *     "month" => 월
     *     ,"day" => 일
     *     ,"hour" => 시
     * ]
     */
    private function getMtExpect() {
        $deadline = 19;
        $proc_time = 0;
        $after_time = 0;

        $hour = date('G');
        $min  = date('i');

        $hm = intval($hour . $min);

        if ($hm > 1900) {
            $proc_time = 24;
        }

        $proc_time += 20;

        return $this->calcExpect($deadline, $proc_time, $after_time);
    }

    /**
     * @brief 그린백 예상출고일 계산
     *
     * @return [
     *     "month" => 월
     *     ,"day" => 일
     *     ,"hour" => 시
     * ]
     */
    private function getGbExpect() {
        $deadline = 19;
        $proc_time = 0;
        $after_time = 0;

        $hour = date('G');
        $min  = date('i');

        $hm = intval($hour . $min);

        if ($hm > 1900) {
            $proc_time = 24;
        }

        $proc_time += 120;

        return $this->calcExpect($deadline, $proc_time, $after_time);
    }

    /**
     * @brief 기타인쇄/자석판촉물 예상출고일 계산
     *
     * @return [
     *     "month" => 월
     *     ,"day" => 일
     *     ,"hour" => 시
     * ]
     */
    private function getEtcExpect() {
        $deadline = 19;
        $proc_time = 0;
        $after_time = 0;

        $hour = date('G');
        $min  = date('i');

        $hm = intval($hour . $min);

        if ($hm > 1900) {
            $proc_time = 24;
        }

        if ($this->sortcode_m === "008001") {
            $proc_time += 96;
        } else {
            $proc_time += 120;
        }

        return $this->calcExpect($deadline, $proc_time, $after_time);
    }

    /**
     * @brief 휴일 추가시간 계산
     *
     * @detail 공휴일 추가시간
     *  (1) from <= x <= to에 해당하는 기간에 공휴일 개수 검색
     *  (2) (1) * 24로 공휴일 추가시간을 더해준다
     *
     * @return 휴일 추가 시간
     */
    private function getHolidayTime($from, $to): int {
        $conn = $this->conn;
        $dao  = $this->productDAO;

        return $dao->selectHoliInfoCount($conn, $from, $to) * 24;
    }

    /**
     * @brief 예상시간 계산 공통함수
     *
     * @detail 공휴일 추가시간
     *  (1) from <= x <= to에 해당하는 기간에 공휴일 개수 검색
     *  (2) (1) * 24로 공휴일 추가시간을 더해준다
     *
     * @return 휴일 추가 시간
     */
    private function calcExpect($deadline, $proc_time, $after_time): array {
        $util = $this->dateUtil;
        // 추가 소요일 검사용
        $cur_stamp = mktime($deadline, 0, 0, date("n"), date("j"), date("Y"));
        $temp = ($proc_time + $after_time) * 3600;

        // 주말 추가시간 계산
        $from = date("Y-m-d", $cur_stamp);
        $to   = date("Y-m-d", $cur_stamp + $temp);
        $weekend_count = $util->getWeekendCount($from, $to);

        $weekend_time = 0;
        if ($weekend_count > 0) {
            $weekend_time = ceil($weekend_count / 2) * 48;
        }

        // 공휴일 추가시간 계산
        $holiday_time = $this->getHolidayTime($from, $to);

        // 예상시간
        $temp += ($weekend_time + $holiday_time) * 3600;
        $expec_time = explode(' ', date("Y m d D H i s", $cur_stamp + $temp));

        return [
             "year"    => $expec_time[0]
            ,"month"   => $expec_time[1]
            ,"day"     => $expec_time[2]
            ,"day_txt" => $expec_time[3]
            ,"hour"    => $expec_time[4]
            ,"min"     => $expec_time[5]
            ,"sec"     => $expec_time[6]
        ];
    }
}
