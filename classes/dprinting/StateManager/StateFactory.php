<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-10-27
 * Time: 오후 7:12
 */


foreach (glob(INC_PATH . "/classes/dprinting/StateManager/concretes/*.php") as $filename)
{
    include_once($filename);
    //echo $filename;
}

class StateFactory {
    public static function createState($state)
    {
        switch ($state) {
            case '2220' :
                return new OutputWaitingState();
                break;
            case '2230' :
                return new OutputingState();
                break;
            case '2260' :
                return new OutputHoldingState();
                break;
            case '2320' :
                return new PrintWaitingState();
                break;
            case '2330' :
                return new PrintingState();
                break;
            case '2360' :
                return new PrintHoldingState();
                break;
            case '2420' :
                return new BasicAfterProcessWaitingState();
                break;
            case '2430' :
                return new BasicAfterProcessingState();
                break;
            case '2460' :
                return new BasicAfterProcessHoldingState();
                break;
            case '2520' :
                return new AfterProcessWaitingState();
                break;
            case '2530' :
                return new AfterProcessingState();
                break;
            case '2560' :
                return new AfterProcessHoldingState();
                break;
            case '2130' :
                //return new OutputingState();
                break;
            case '3120' :
                return new InwarehouseWatingState();
                break;
            case '3220' :
                return new ReleaseWaitingState();
                break;
            case '3320' :
                return new DeliveryWaitingState();
                break;
            default:
                return null;
                break;
        }
    }
}