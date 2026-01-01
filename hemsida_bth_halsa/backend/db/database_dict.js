// backend/db/question_temp.js
const tempDb = {
    units: [
        { id: 1, ascii_name: "%%second_prefix%%g", accepted_answer: ["%%second_prefix%%g", "g", "gram", "grams", "gr", "g.", "gr.", "g ", " gram", "grams ", "gr ", "gr. "] },
        { id: 2, ascii_name: "%%second_prefix%%l", accepted_answer: ["%%second_prefix%%l", "l", "liter", "liters", "litre", "litres", "l.", "l ", "liter ", "liters ", "litre ", "litres "] },
        { id: 3, ascii_name: "ml", accepted_answer: ["ml", "milliliter", "milliliters", "millilitre", "millilitres", "ml.", "ml ", "milliliter ", "milliliters ", "millilitre ", "millilitres ", "mL", "mL."] },
        { id: 4, ascii_name: "mg", accepted_answer: ["mg", "milligram", "milligrams", "mg.", "mg ", "milligram ", "milligrams ", "mgm"] },
        { id: 5, ascii_name: "tablett", accepted_answer: ["tablett", "tabletter", "tbl", "tbl.", "tab", "tab.", "tabs", "tabs.", "tablet", "tablets", "tablett ", "tabletter ", "tbl ", "tbl. ", "tab ", "tab. ", "tabs ", "tabs. ", "tablet ", "tablets "] },
        { id: 6, ascii_name: "enheter", accepted_answer: ["enheter", "e", "enhet", "enheter ", "e ", "enhet "] },
        { id: 7, ascii_name: "mg/dostillfällen", accepted_answer: ["mg", "mg/dostillfällen", "mg/dostillfalle", "mg per dostillfälle", "mg per dostillfalle", "mg/dose occasion", "mg/dose", "mg/dos", "mg/dostillfällen ", "mg/dostillfalle ", "mg per dostillfälle ", "mg per dostillfalle ", "mg/dose occasion ", "mg/dose ", "mg/dos "] },
        { id: 8, ascii_name: "ampuller", accepted_answer: ["ampull", "ampuller", "amp", "amps", "amp.", "amps.", "ampull ", "ampuller ", "amp ", "amps ", "amp. ", "amps. "] },
        { id: 9, ascii_name: "antal", accepted_answer: ["antal", "st", "styck", "stycken", "pcs", "piece", "pieces", "number", "count", "antal ", "st ", "styck ", "stycken ", "pcs ", "piece ", "pieces ", "number ", "count ", ""] },
        { id: 10, ascii_name: "mg/dygn", accepted_answer: ["mg/dygn", "mg/dag", "mg per dygn", "mg per dag", "mg/day", "mg/24h", "mg per 24h", "mg/dygn ", "mg/dag ", "mg per dygn ", "mg per dag ", "mg/day ", "mg/24h ", "mg per 24h "] },
        { id: 11, ascii_name: "g", accepted_answer: ["g", "gram", "grams", "gr", "g.", "gr.", "g ", "gram ", "grams ", "gr ", "gr. "] },
        { id: 12, ascii_name: "min", precision: 0, accepted_answer: ["min", "minut", "minuter", "minute", "minutes", "min.", "min ", "minut ", "minuter ", "minute ", "minutes "] },
        { id: 13, ascii_name: "droppar/min", precision: 0, accepted_answer: ["droppar/min", "droppar/minut", "droppar/minuter", "droppar/m", "drops/min", "drops/minute", "drops/minutes", "drop/min", "drop/minute", "drop/minutes", "droppar/min ", "droppar/minut ", "droppar/minuter ", "droppar/m ", "drops/min ", "drops/minute ", "drops/minutes ", "drop/min ", "drop/minute ", "drop/minutes "] },
        { id: 14, ascii_name: "ml/h", precision: 0, accepted_answer: ["ml/h", "ml per h", "ml per timme", "ml/timme", "ml/hr", "ml/hour", "milliliter per hour", "milliliters per hour", "ml/h ", "ml per h ", "ml per timme ", "ml/timme ", "ml/hr ", "ml/hour ", "milliliter per hour ", "milliliters per hour "] },
    ],

    courses: [
        { course_code: "KM1423", course_name: "Mikrobiologi", question_types: '["Enhetsomvandling", "Dos, Styrka, Mängd"]' },
        { course_code: "KM1424", course_name: "Patofysiologi I", question_types: '["Enhetsomvandling", "Dos, Styrka, Mängd", "Tillsats", "Stamlösning"]' },
        { course_code: "KM1425", course_name: "Patofysiologi II", question_types: '["Enhetsomvandling", "Dos, Styrka, Mängd", "Tillsats", "Stamlösning"]' },
        { course_code: "OM1541", course_name: "Omvårdnad vid akuta situationer och komplexa ohälsotillstånd", question_types: '["Enhetsomvandling", "Dos, Styrka, Mängd", "Tillsats", "Stamlösning"]' }
    ],

    formulas: [
        { name: "Nelson's Formula", formula: "2 * age + 8", input: '["age"]', output: '["weight"]', units : '["kg"]', description: "Nelson's formula is used to estimate the weight of a child based on their age." },
        { name: "child_weight", formula: "round( age < 5 ? 15 + (age - 3) * 2.5 : age < 7 ? 20 + (age - 5) * 2.5 : age < 9 ? 25 + (age - 7) * 2.5 : age < 12 ? 30 + (age - 9) * 3.33 : 40 + (age - 12) * 5, 1)", input: '["age"]', output: '["weight"]', units : '["kg"]', description: "This formula estimates the weight of a child based on their age." },
    ],

    qtypes: [
        { id: 0, name: "Tillsats"},
        { id: 1, name: "Enhetsomvandling"},
        { id: 2, name: "Dos, Styrka, Mängd"},
        { id: 3, name: "Stamlösning"},
        { id: 4, name: "Gaser"},
        { id: 6, name: "Infusionshastighet"}
    ],

    medicine: [
        { id: 1, namn: "Morfin", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=20131019000027", variating_values: '{"dosage": [15, 10], "available_dose": [10], "injektions_styrka": [10], "dos": [10]}' },
        { id: 2, namn: "Digoxin", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=19741206000019", variating_values: '{"tablet_dose": [0.25, 0.13], "dosage": [0.25], "available_dose": [0.25]}' },
        { id: 3, namn: "Neostigmin", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=19430311000013", variating_values: '{"dosage": [0.5], "available_dose": [1.0]}' },
        { id: 4, namn: "Heminevrin", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=19680321000019", variating_values: '{"dosage": [5, 20, 1], "available_dose": [50]}' },
        { id: 5, namn: "Kåvepenin", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=19790831000024", variating_values: '{"dosage": [12.5, 25, 50]}' },
        { id: 6, namn: "Novorapid", fass_link: "https://www.fass.se/LIF/result?query=Novorapid&userType=0", variating_values: '{"dosage": [0.04, 0.5, 0.01], "available_dose": [100]}' },
        { id: 7, namn: "Doktacillin", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=19640331000010", variating_values: '{"ordinerad_mangd": [50], "antal_doser": [4]}' },
        { id: 8, namn: "Dalacin", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=19760608000014", variating_values: '{"available_dose": [150], "ampul_mangd": [4, 2], "ordinerad_mangd_infusion": [600], "antal_dostillfallen": [3], "dosage": [20]}' },
        { id: 9, namn: "Klexane", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=20170530000178", variating_values: '{"ordinerad_mangd": [100], "ampul_styrka": [100000, 30000], "ampul_mangd": [10, 3]}' },
        { id: 10, namn: "Paracetamol", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=20110119000014", variating_values: '{"infusions_styrka": [10], "dos": [10]}' },
        { id: 11, namn: "Ventoline", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=19731005000020", variating_values: '{"styrka": [5, 2], "ordinerad_mangd_nebulisator": [0.5, 2, 0.5]}' },
        { id: 12, namn: "Droperidol", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=20120405000017", variating_values: '{"injektions_styrka": [2.5], "ampul_mangd": [2], "ordinerad_mangd_injektionsvatska": [0.625, 0.75, 1, 1.25]}' },
        { id: 13, namn: "Syrgas", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=20040607011584", variating_values: '{"doserings_hastighet": [1, 10]}' },
        { id: 14, namn: "Ringeracetat", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=20040607004883", variating_values: '{}' },
        { id: 15, namn: "Natriumklorid", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=19900316000184", variating_values: '{}' },
        { id: 16, namn: "Kaliumklorid", fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=19900316000160", variating_values: '{}' },
      ],

    question_data: [
        {
            question: "En patient är ordinerad %%medicine[0].namn%% %%medicine[0].dosage%% ml injektionsvätska insulin med styrkan %%medicine[0].available_dose%% E/ml.\nHur många E motsvarar detta?",
            answer_unit_id: 6,
            answer_formula: "medicine[0].dosage * medicine[0].available_dose",
            variating_values: "{\"medicine.namn\": [\"Novorapid\"]}",
            course_codes: ["KM1423", "KM1424", "KM1425", "OM1541"],
            question_type_id: 2,
            hints: []
        },
        {
            question: "%%name%% ordineras %%medicine[0].dosage%% mg %%medicine[0].namn%%/kg kroppsvikt/dos. Patienten väger %%weight%% kg.\nHur många mg erhåller patienten per dostillfälle?",
            answer_unit_id: 4,
            answer_formula: "medicine[0].dosage * weight",
            variating_values: "{\"medicine.namn\": [\"Kåvepenin\"], \"weight\": [50,150,1]}",
            course_codes: ["KM1423", "KM1424", "KM1425", "OM1541"],
            question_type_id: 2,
            hints: ["Kom ihåg att ta hänsyn till vikten när du räknar ut dosen."]
        },
        {   
            question: "Din patient är ordinerad %%medicine[0].dosage%% ml %%medicine[0].namn%% %%medicine[0].available_dose%% mg/ml, oral lösning.\nHur många milligram motsvarar detta?",
            answer_unit_id: 4,
            answer_formula: "medicine[0].dosage * medicine[0].available_dose",
            variating_values: "{\"medicine.namn\": [\"Heminevrin\"]}",
            course_codes: ["KM1423", "KM1424", "KM1425", "OM1541"],
            question_type_id: 2,
            hints: [] 
        },  
        {
            preamble: "En patient har en postoperativ sårinfektion. Patienten ordineras infusion %%medicine[0].namn%% %%medicine[0].ordinerad_mangd_infusion%% mg x %%medicine[0].antal_dostillfallen%% . I medicinskåpet finner du injektionsvätska %%medicine[0].namn%% %%medicine[0].available_dose%% mg/ml med %%medicine[0].ampul_mangd%% ml i varje ampull.",
            question_data: [
                {
                    question: "\nHur många ampuller behöver du använda vid ett dostillfälle?",
                    answer_unit_id: 8,
                    answer_formula: "medicine[0].ordinerad_mangd_infusion / (medicine[0].available_dose * medicine[0].ampul_mangd)",
                    hints: ["Tänka på att svaret är antalet ampuller, och ampullen innehåller mer än 1 ml."]
                },
                {
                    question: "\nVilken blir den totala dygnsdosen %%medicine[0].namn%% som patienten får?",
                    answer_unit_id: 4,
                    answer_formula: "medicine[0].ordinerad_mangd_infusion * medicine[0].antal_dostillfallen",
                    hints: ["Det är flera dostillfällen per dygn."]
                }
            ],
            variating_values: "{\"medicine.namn\": [\"Dalacin\"]}",
            course_codes: ["KM1423", "KM1424", "KM1425", "OM1541"],
            question_type_id: 2
        },
        {
            preamble: "Ett barn har fått en postoperativ sårinfektion. Patienten ordineras infusion %%medicine[0].namn%% %%medicine[0].dosage%% mg/kg och dygn fördelat på %%medicine[0].antal_dostillfallen%% dostillfällen. Barnet väger %%weight%% kg.",
            question_data: [
                {
                    question: "\nHur många milligram motsvarar ett dostillfälle? Ange svaret i hela milligram.",
                    answer_unit_id: 4,
                    answer_formula: "medicine[0].dosage * weight / medicine[0].antal_dostillfallen",
                    hints: [
                        "Tänk på att svaret är i milligram.",
                        "Tänk på att svaret är per dostillfälle."
                    ]
                },
                {
                    question: "\nVilken blir den totala dygnsdosen %%medicine[0].namn%% som patienten får? Ange svaret i gram.",
                    answer_unit_id: 11,
                    answer_formula: "(medicine[0].dosage * weight) / 1000",
                    hints: ["Tänk på att svaret är i gram."]
                }
            ],
            variating_values: "{\"medicine.namn\": [\"Dalacin\"], \"weight\": [20, 50]}",
            course_codes: ["KM1423", "KM1424", "KM1425", "OM1541"],
            question_type_id: 2
        },
        {   // GOOD
            preamble: "Patienten är ordinerad infusion %%medicine[0].namn%%. Tillgängligt finns %%medicine[0].namn%% infusionsvätska, lösning %%medicine[0].infusions_styrka%% mg/ml. Patientens vikt är %%weight%% kg och ska därför få %%medicine[0].dos%% mg/kg.",
            question_data: [
                {
                    question: "\nHur många mg får patienten?",
                    answer_unit_id: 4,
                    answer_formula: "medicine[0].dos * weight",
                    hints: [
                        "Här eftersöks en dos, men du behöver inte använda triangeln.",
                        "Tänk på att svaret är i milligram."
                    ]
                },
                {
                    question: "\nHur många ml får patienten?",
                    answer_unit_id: 3,
                    answer_formula: "(medicine[0].dos * weight) / medicine[0].infusions_styrka",
                    hints: [
                        "Räkna först ut dosen patienten ska ha",
                        "Tänk på att svaret är i milliliter."
                    ]
                }
            ],
            variating_values: "{\"medicine.namn\": [\"Paracetamol\"], \"weight\": [10, 50]}",
            course_codes: ["KM1423", "KM1424", "KM1425", "OM1541"],
            question_type_id: 2
        },
        {
            preamble: "Läkaren har ordinerat intramuskulär injektion %%medicine[0].namn%% %%medicine[0].injektions_styrka%% mg x %%antal%%. Tillgängligt finns %%medicine[0].namn%% injektionsvätska %%medicine[0].dos%% mg/ml.",
            question_data: [
                {
                    question: "\nHur många ml motsvarar ett dostillfälle?",
                    answer_unit_id: 3,
                    answer_formula: "medicine[0].injektions_styrka / medicine[0].dos",
                    hints: ["Här eftersöks en mängd."]
                },
                {
                    question: "\nHur många mg får patienten vid varje dostillfälle?",
                    answer_unit_id: 4,
                    answer_formula: "medicine[0].injektions_styrka * antal",
                    hints: [
                        "Här behöver du inte använda triangeln",
                        "Tänk på att svaret är i milligram."
                    ]
                }
            ],
            variating_values: "{\"medicine.namn\": [\"Morfin\"], \"antal\": [1, 3]}",
            course_codes: ["KM1423", "KM1424", "KM1425", "OM1541"],
            question_type_id: 2
        },
        {
            question: "%%namn%% har kronisk bronkobstruktion och ordineras inhalation med %%medicine[0].namn%%. %%medicine[0].namn%% är ett bronkdilaterande medel vid astma och KOL som ges via nebulisator.\nTillgängligt preparat: %%medicine[0].namn%%, lösning för nebulisator %%medicine[0].styrka%% mg/ml.\nHur många mg erhåller %%namn%% om %%medicine[0].ordinerad_mangd_nebulisator%% ml %%medicine[0].namn%% ordineras?",
            answer_unit_id: 4,
            answer_formula: "medicine[0].styrka * medicine[0].ordinerad_mangd_nebulisator",
            variating_values: "{\"medicine.namn\": [\"Ventoline\"]}",
            course_codes: ["KM1423", "KM1424", "KM1425", "OM1541"],
            question_type_id: 2,
            hints: ["Här eftersöks en dos."]
        },
        {
            question: "En patient med illamående ordineras %%medicine[0].namn%% intravenöst. Tillgängligt preparat är %%medicine[0].namn%%, injektionsvätska, lösning %%medicine[0].injektions_styrka%% mg/ml.\nAmpullen innehåller %%medicine[0].ampul_mangd%% ml. Patienten ordineras %%medicine[0].ordinerad_mangd_injektionsvatska%% mg.\nHur många ml injektionsvätska motsvarar det?",
            answer_unit_id: 3,
            answer_formula: "medicine[0].ordinerad_mangd_injektionsvatska / medicine[0].injektions_styrka",
            variating_values: "{\"medicine.namn\": [\"Droperidol\"]}",
            course_codes: ["KM1423", "KM1424", "KM1425", "OM1541"],
            question_type_id: 2,
            hints: ["Här eftersöks en mängd"]
        },
        {
            question: "Din patient är i behov av %%medicine[0].namn%% och är därför ordinerad %%medicine[0].doserings_hastighet%% liter/minut.\n" +
                       "Patienten ska nu på en röntgenundersökning och behöver då syrgastillförsel under transporten.\n" +
                       "Du hämtar en transportabel syrgasflaska som har en volym på %%Volym%% liter och ett tryck på %%Tryck%% bar.\n" +
                       "Hur länge räcker syrgasen?\nUtgå från att patientens syrgasbehov förblir oförändrat. Ange svaret i hela minuter.",
            answer_unit_id: 12,
            answer_formula: "(Volym * Tryck) / medicine[0].doserings_hastighet",
            variating_values: "{\"medicine.namn\": [\"Syrgas\"], \"Volym\": [2, 5, 10], \"Tryck\": [50, 200, 1]}",
            course_codes: ["OM1541"],
            question_type_id: 5,
            hints: [
                "Hur många liter komprimerad syrgas finns i flaskan?",
            ]
        },
        {
            question: "%%namn%% har en pneumoni och behöver %%medicine[0].namn%% på grimma, %%syrgasflode%% liter %%medicine[0].namn%% per minut.\n" +
                        "Ni ska nu lämna avdelningen och behöver ha med en transportabel syrgasflaska.\n" +
                        "Tillgängligt finns en syrgasflaska på %%volym%% liter med trycket %%tryck%% bar.\n" +
                        "Hur länge räcker syrgasen?\nUtgå från att patientens syrgasbehov förblir oförändrat.\n" +
                        "Ange svaret i hela minuter.",
            answer_unit_id: 12,
            answer_formula: "(volym * tryck) / syrgasflode",
            variating_values: "{\"medicine.namn\": [\"Syrgas\"], \"syrgasflode\": [0.5, 3.0, 0.5], \"volym\": [2, 5, 10], \"tryck\": [50, 200, 1]}",
            course_codes: ["OM1541"],
            question_type_id: 5,
            hints: [
                "Hur många liter komprimerad syrgas finns i flaskan?"
            ]
        },
        {
            question: "En patient ordineras %%volym%% ml %%medicine[0].namn%%.\nInfusionen påbörjas 0%%start%%:00 och ska vara avslutad 1%%end%%:00.\n" +
                        "Vilken infusionshastighet i droppar/min innebär det?\nAnge svaret i hela droppar/min.",
            answer_unit_id: 13,
            answer_formula: "(volym * 20) / ((end + 10 - start) * 60)",
            variating_values: "{\"medicine.namn\": [\"Ringeracetat\"], \"volym\": [500, 1000, 100], \"start\": [1, 9, 1], \"end\": [3, 9, 1]}",
            course_codes: ["OM1541", "KM1425"],
            question_type_id: 6,
            hints: [
                "Hur många droppar motsvarar infusionen? (1 ml = 20 droppar)",
                "Hur många minuter tar infusionen totalt?"
            ]
        },
        {
            question: "En patient ordineras %%volym%% ml %%medicine[0].namn%%.\nInfusionen påbörjas 0%%start%%:00 och ska vara avslutad 1%%end%%:00.\n" +
                        "Vilken hastighet i ml/h innebär det?\nAnge svaret i hela ml/h.",
            answer_unit_id: 14,
            answer_formula: "volym / (end + 10 - start)",
            variating_values: "{\"medicine.namn\": [\"Ringeracetat\"], \"volym\": [500, 1000, 100], \"start\": [1, 9, 1], \"end\": [3, 9, 1]}",
            course_codes: ["OM1541", "KM1425"],
            question_type_id: 6,
            hints: [
                "Hur många timmar är det mellan klockslagen?",
                "Dela den totala volymen med antalet timmar."
            ]
        },
        // GIGA BAD QUESTION, CAN'T FIGURE IT OUT
        // {
        //     question: "Din patient ordineras infusion %%medicine[0].namn%% %%volym%% ml med en dropptakt på %%dropptakt%% droppar/minut. " +
        //                 "Vid vilket klockslag beräknas infusionen vara färdig om infusionen påbörjas kl. %%starttid%%:00? (1 ml = 20 droppar)",
        //     answer_unit_id: 0,
        //     answer_formula: "((volym * 20) / dropptakt) / 60",
            
        //     "starttid + (volym * 20 / dropptakt) / 60",
        //     variating_values: "{\"medicine.namn\": [\"Ringeracetat\"], \"volym\": [400, 2000, 100], \"dropptakt\": [60, 100, 5], \"starttid\": [1, 12, 1]}",
        //     course_codes: ["OM1541"],
        //     question_type_id: 3,
        //     hints: [
        //         "Räkna ut totala droppar: volym × 20",
        //         "Dela med droppfaktor (droppar/min) för att få minuter",
        //         "Översätt minuter till timmar (min/60) och addera till starttiden"
        //     ]
        // },
        {
            // Volym 500
            question: 
                "%%glukos_volym%% ml av infusionsvätska Glukos %%glukos_koncentration%% mg/ml ordineras till en patient med parenteral nutrition." +
                "Du gör följande tillsatser till infusionslösningen: %%natriumklorid_mmol%% mmol %%medicine[0].namn%% (%%natriumklorid_koncentration%% mmol/ml)" +
                "och %%kaliumklorid_mmol%% mmol %%medicine[1].namn%% (%%kaliumklorid_koncentration%% mmol/ml).\n" +
                "Infusionen ska ges på %%timmar%% timmar.\nVilken blir infusionshastigheten i ml/h?\nAnge svaret i hela ml/h.",
            answer_unit_id: 14,
            answer_formula: "((natriumklorid_mmol / natriumklorid_koncentration) + (kaliumklorid_mmol / kaliumklorid_koncentration) + glukos_volym) / timmar",
            variating_values: 
                "{\"medicine.namn\":[\"Natriumklorid\", \"Kaliumklorid\"]," +
                "\"glukos_koncentration\": [\"50\", \"100\"]," +
                "\"glukos_volym\": [500]," +
                "\"natriumklorid_mmol\":[10, 80, 5],\"natriumklorid_koncentration\":[4]," +
                "\"kaliumklorid_mmol\":[10, 30, 5],\"kaliumklorid_koncentration\":[1, 2]," +
                "\"timmar\":[6, 12, 0.5]}",
            course_codes: ["OM1541", "KM1425"],
            question_type_id: 6,
            hints: [
                "Räkna först ut volymen NaCl (mmol / mmol/ml) och KCl (mmol / mmol/ml).",
                "Lägg ihop volymen för tillsatsen tillsatserna med volymen i infusionspåsen.",
                "Dela totalvolymen med antalet timmar.",
            ]
        },
        {    
            // Volym 1000
            question: 
                "%%glukos_volym%% ml av infusionsvätska Glukos %%glukos_koncentration%% mg/ml ordineras till en patient med parenteral nutrition." +
                "Du gör följande tillsatser till infusionslösningen: %%natriumklorid_mmol%% mmol %%medicine[0].namn%% (%%natriumklorid_koncentration%% mmol/ml)\n" +
                "och %%kaliumklorid_mmol%% mmol %%medicine[1].namn%% (%%kaliumklorid_koncentration%% mmol/ml).\n" +
                "Infusionen ska ges på %%timmar%% timmar.\nVilken blir infusionshastigheten i ml/h?\nAnge svaret i hela ml/h.",
            answer_unit_id: 14,
            answer_formula: "((natriumklorid_mmol / natriumklorid_koncentration) + (kaliumklorid_mmol / kaliumklorid_koncentration) + glukos_volym) / timmar",
            variating_values:
                "{\"medicine.namn\":[\"Natriumklorid\", \"Kaliumklorid\"]," +
                "\"glukos_koncentration\": [\"50\", \"100\"]," +
                "\"glukos_volym\": [1000]," +
                "\"natriumklorid_mmol\":[10, 160, 5],\"natriumklorid_koncentration\":[4]," +
                "\"kaliumklorid_mmol\":[10, 60, 5],\"kaliumklorid_koncentration\":[1, 2]," +
                "\"timmar\":[6, 12, 0.5]}",
            course_codes: ["OM1541", "KM1425"],
            question_type_id: 6,
            hints: [
                "Räkna först ut volymen NaCl (mmol / mmol/ml) och KCl (mmol / mmol/ml).",
                "Lägg ihop volymen för tillsatsen tillsatserna med volymen i infusionspåsen.",
                "Dela totalvolymen med antalet timmar.",
            ]
        },
        {
            // Volym 500
            question:
                "%%glukos_volym%% ml av infusionsvätska Glukos %%glukos_koncentration%% mg/ml ordineras till en patient med parenteral nutrition." +
                "Du gör följande tillsatser till infusionslösningen: %%natriumklorid_mmol%% mmol %%medicine[0].namn%% (%%natriumklorid_koncentration%% mmol/ml) " +
                "och %%kaliumklorid_mmol%% mmol %%medicine[1].namn%% (%%kaliumklorid_koncentration%% mmol/ml).\n" +
                "Infusionen ska ges på %%timmar%% timmar.\nVilken blir infusionshastigheten i droppar/minut? (1 ml = 20 droppar).\nAnge svaret i hela droppar/minut.",
            answer_unit_id: 13,
            answer_formula: "(((natriumklorid_mmol / natriumklorid_koncentration) + (kaliumklorid_mmol / kaliumklorid_koncentration) + glukos_volym) * 20) / timmar",
            variating_values:
                "{\"medicine.namn\":[\"Natriumklorid\", \"Kaliumklorid\"]," +
                "\"glukos_koncentration\": [\"50\", \"100\"]," +
                "\"glukos_volym\": [500]," +
                "\"natriumklorid_mmol\":[10, 80, 5],\"natriumklorid_koncentration\":[4]," +
                "\"kaliumklorid_mmol\":[10, 30, 5],\"kaliumklorid_koncentration\":[1, 2]," +
                "\"timmar\":[6, 12, 0.5]}",
            course_codes: ["OM1541", "KM1425"],
            question_type_id: 6,
              hints: [
                "Räkna ut totalvolymen efter tillsats först.",
                "Hur många droppar finns det i påsen?",
            ]
        },
        {
            // Volym 1000
            question:
                "%%glukos_volym%% ml av infusionsvätska Glukos %%glukos_koncentration%% mg/ml ordineras till en patient med parenteral nutrition." +
                "Du gör följande tillsatser till infusionslösningen: %%natriumklorid_mmol%% mmol %%medicine[0].namn%% (%%natriumklorid_koncentration%% mmol/ml) " +
                "och %%kaliumklorid_mmol%% mmol %%medicine[1].namn%% (%%kaliumklorid_koncentration%% mmol/ml).\n" +
                "Infusionen ska ges på %%timmar%% timmar.\nVilken blir infusionshastigheten i droppar/minut? (1 ml = 20 droppar).\nAnge svaret i hela droppar/minut.",
            answer_unit_id: 13,
            answer_formula: "(((natriumklorid_mmol / natriumklorid_koncentration) + (kaliumklorid_mmol / kaliumklorid_koncentration) + glukos_volym) * 20) / timmar",
            variating_values:
                "{\"medicine.namn\":[\"Natriumklorid\", \"Kaliumklorid\"]," +
                "\"glukos_koncentration\": [\"50\", \"100\"]," +
                "\"glukos_volym\": [1000]," +
                "\"natriumklorid_mmol\":[10, 160, 5],\"natriumklorid_koncentration\":[4]," +
                "\"kaliumklorid_mmol\":[10, 60, 5],\"kaliumklorid_koncentration\":[1, 2]," +
                "\"timmar\":[6, 12, 0.5]}",
            course_codes: ["OM1541", "KM1425"],
            question_type_id: 6,
              hints: [
                "Räkna ut totalvolymen efter tillsats först.",
                "Hur många droppar finns det i påsen?",
            ]
        },
        {
            question:
                "Hur lång tid tar det att infundera %%infusions_vätska%% infusionsvätska med en infusionshastighet av %%dropphastighet%% droppar/min.\n" +
                "På aggregatet ser du att 1ml = 20 droppar.\nSvara i hela minuter.",
            answer_unit_id: 12,
            answer_formula: "(infusions_vätska * 20) / dropphastighet",
            variating_values:
                "{\"infusions_vätska\": [100, 1000, 100]," +
                "\"dropphastighet\": [30, 100]}",
            course_codes: ["OM1541", "KM1425"],
            question_type_id: 6,
            hints: [
                "Räkna ut antalet droppar för hela volymen först",
            ]
        },
    ]




};

module.exports = tempDb;



 // {
        //     preamble: "%%namn%% har en övre urinvägsinfektion och är ordinerad %%medicine[0].namn%%. Tillgängligt preparat är %%medicine[0].namn%% pulver till injektions-/infusionsvätska, lösning. %%namn%% är %%age%% år, ordineras %%medicine[0].namn%% som intravenös injektion. %%namn%% väger %%weight%% kg. Ordinerad mängd är %%medicine[0].ordinerad_mangd%% mg/kg kroppsvikt och dygn fördelat på %%medicine[0].antal_doser%% doser. Varje dos blandas med %%utspadning%% ml vatten för injektionsvätskor.",
        //     question_data: [
        //         {
        //             question: "Hur många gram blir dygnsdosen?",
        //             answer_unit_id: 10,
        //             answer_formula: "medicine[0].ordinerad_mangd * weight / utspadning / medicine[0].antal_doser",
        //             hints: [
        //                 "Här behöver du inte använda dos/styrka/mängd-triangeln.",
        //                 "Här är det den totala dosen på dygnet vi vill veta."
        //             ]
        //         },
        //         {
        //             question: "Hur många mg skall ges vid varje dostillfälle?",
        //             answer_unit_id: 7,
        //             answer_formula: " medicine[0].ordinerad_mangd * weight",
        //             hints: [
        //                 "Här behöver du inte använda dos/styrka/mängd-triangeln.",
        //                 "Tänk på att fördela dygnsdosen på rätt antal dostillfällen."
        //             ]
        //         }
        //     ],
        //     variating_values: "{\"medicine.namn\": [\"Doktacillin\"], \
        //                         \"utspadning\": [10],  \
        //                         \"age\": [3, 14], \
        //                         \"weight\": \"formula(child_weight(age))\"\
        //                         }",
        //     course_codes: ["KM1423", "KM1424", "KM1425", "OM1541"],
        //     question_type_id: 2
        // },



                // {
        //     id: 10,
        //     question: "%%namn%% är ordinerad subcutan inj. %%medicine[0].namn%% %%medicine[0].ordinerad_mangd%% E som trombosprofylax. Just idag finns bara tillgängligt finns inj. %%medicine[0].namn%% i ampuller med %%medicine[0].ampul_styrka%%E/%%medicine[0].ampul_mangd%% ml. Hur många ml ger du %%namn%%?",
        //     answer_formula: "ordinerad_mangd / (ampul_styrka / ampul_mangd)",
        //     answer_unit_id: 3,
        //     variating_values: "{\"medicine\": [\"Klexane\"], \
        //                         \"ordinerad_mangd\": [\"100 000E/10 ml\", \"30 000 E/3 ml\"], \
        //                         \"ampul_styrka\": \"100000 ? ordinerad_mangd == \'100 000E/10 ml\' : 30000\", \
        //                         \"ampul_mangd\": \"10 ? ordinerad_mangd == \'100 000E/10 ml\' : 3\" \
        //                         }",
        //     course_codes: ["KM1423", "KM1424", "KM1425", "OM1541"],
        //     question_type_id: 2,
        //     hints: [
        //         "Räkna ut styrka per milliliter först.",
        //         "Subcutana injektioner är ofta små mängder.",
        //     ]
        // },
