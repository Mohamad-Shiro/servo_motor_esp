#ifndef Arduino_h
#include <Arduino.h>
#endif

String webPageCode = R"(
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta http-equiv="X-UA-Compatible" content="IE=edge" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>Servo Motor Control</title>
        <style>
            * {
                margin: 0;
                padding: 0;
                font-family: monospace;
                -webkit-box-sizing: border-box;
                -moz-box-sizing: border-box;
                box-sizing: border-box;
            }
            section {
                width: 485px;
                margin: 0 auto;
                text-align: center;
                margin-top: 20px;
                border-radius: 10px;
                background-color: #fff;
                box-shadow: rgba(0, 0, 0, 0.35) 0px 5px 15px;
            }
            section .info {
                padding: 15px;
                color: #0075ff;
                font-size: 18px;
            }
            section .info span {
                border: 1px solid #0075ff;
                border-radius: 5px;
                width: 40px;
                display: inline-block;
            }
            section h1 {
                padding: 10px 0;
                font-size: 28px;
                text-transform: capitalize;
            }
            section .info {
                display: flex;
                justify-content: space-around;
            }
            .controls {
                font-size: 18px;
            }
            .control-item {
                display: grid;
                grid-template-columns: 0.5fr 1fr 0.5fr;
                gap: 10px;
                padding: 10px 15px;
                color: #0075ff;
                align-items: center;
            }
            .control-item > div:nth-child(2) {
                display: flex;
                align-items: center;
                justify-content: center;
                gap: 10px;
            }

            .control-item > div:nth-child(2) span {
                border: 1px solid #0075ff;
                border-radius: 5px;
                width: 50px;
                display: inline-block;
                padding: 5px 0;
            }
            .control-item button {
                border: 1px solid #0075ff;
                color: #0075ff;
                background-color: #fff;
                padding: 10px;
                border-radius: 10px;
                width: 100%;
                font-size: 15px;
                font-weight: bold;
                transition: all 0.2s ease-in-out;
            }
            .control-item button:hover {
                background-color: #0075ff;
                color: #fff;
                cursor: pointer;
            }

            /* Input design */

            input[type='range'] {
                height: 25px;
                -webkit-appearance: none;
                -moz-appearance: none;
                margin: 10px 0;
                width: 100%;
            }
            input[type='range']:focus {
                outline: none;
            }
            input[type='range']::-webkit-slider-runnable-track {
                width: 100%;
                height: 10px;
                cursor: pointer;
                background: #e8e7e7;
                border-radius: 5px;
                border: 1px solid #494949;
            }
            input[type='range']::-webkit-slider-thumb {
                height: 18px;
                width: 18px;
                border-radius: 50%;
                background: #0075ff;
                cursor: pointer;
                -webkit-appearance: none;
                margin-top: -6px;
            }

            /* Firefox */
            input[type='range']::-moz-slider-runnable-track {
                width: 100%;
                height: 10px;
                cursor: pointer;
                background: #ccc;
                border-radius: 5px;
                border: 1px solid #333;
            }
            input[type='range']::-moz-slider-thumb {
                height: 18px;
                width: 18px;
                border-radius: 50%;
                background: #0075ff;
                cursor: pointer;
                -moz-appearance: none;
                margin-top: -6px;
            }
        </style>
    </head>
    <body>
        <section class="content">
            <h1>servo motor control</h1>
            <div class="info">
                <div>
                    <p>Current position: <span id="curr-pos">0</span></p>
                </div>
                <div>
                    <p>
                        Current buttons speed:
                        <span id="curr-btn-speed">5</span>
                    </p>
                </div>
            </div>
            <div class="controls">
                <div class="control-item">
                    <div>Change buttons speed:</div>
                    <div>
                        <input
                            id="chng-btn-speed"
                            type="range"
                            min="0"
                            max="100"
                            value="50"
                            class="value-slider"
                        />
                        <span>0</span>
                    </div>
                    <div>
                        <button id="chng-speed" class="btn">Change</button>
                    </div>
                </div>
                <div class="control-item">
                    <div>Sync position:</div>
                    <div>
                        <input
                            id="sync-pos"
                            type="range"
                            min="0"
                            max="100"
                            value="50"
                            class="value-slider"
                        />
                        <span>0</span>
                    </div>
                    <div></div>
                </div>
                <div class="control-item">
                    <div>Change operation speed:</div>
                    <div>
                        <input
                            id="chng-op-speed"
                            type="range"
                            min="0"
                            max="100"
                            value="50"
                            class="value-slider"
                        />
                        <span>0</span>
                    </div>
                    <div></div>
                </div>
                <div class="control-item">
                    <div>Go to angle:</div>
                    <div>
                        <input
                            id="angle"
                            type="range"
                            min="0"
                            max="100"
                            value="50"
                            class="value-slider"
                        />
                        <span>0</span>
                    </div>
                    <div><button id="go-to-angle" class="btn">Go</button></div>
                </div>
                <div class="control-item">
                    <div>Increase angle by:</div>
                    <div>
                        <input
                            id="inc"
                            type="range"
                            min="0"
                            max="100"
                            value="50"
                            class="value-slider"
                        />
                        <span>0</span>
                    </div>
                    <div>
                        <button id="increase" class="btn">Increase</button>
                    </div>
                </div>
                <div class="control-item">
                    <div>Decrease angle by:</div>
                    <div>
                        <input
                            id="dec"
                            type="range"
                            min="0"
                            max="100"
                            value="50"
                            class="value-slider"
                        />
                        <span>0</span>
                    </div>
                    <div>
                        <button id="decrease" class="btn">Decrease</button>
                    </div>
                </div>
                <div class="control-item">
                    <div>Move From:</div>
                    <div>
                        <input
                            id="move-from"
                            type="range"
                            min="0"
                            max="100"
                            value="50"
                            class="value-slider"
                        />
                        <span>0</span>
                    </div>
                    <div></div>
                </div>
                <div class="control-item">
                    <div>To</div>
                    <div>
                        <input
                            id="move-to"
                            type="range"
                            min="0"
                            max="100"
                            value="50"
                            class="value-slider"
                        />
                        <span>0</span>
                    </div>
                    <div><button id="move" class="btn">Move</button></div>
                </div>
            </div>
        </section>
        
        <script>
            let data = {
                "pos": __posVal,
                "currSpeed": __currSpeedVal
            };
        </script>
    </body>
</html>
)";

