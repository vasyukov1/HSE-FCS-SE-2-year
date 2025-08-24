#let template(cfg: none, body) = {
    assert(cfg != none)
    assert(cfg.project != none)
    assert(cfg.project.name != none)
    assert(cfg.student != none)
    assert(cfg.student.name != none)
    assert(cfg.student.group != none)
    assert(cfg.agreed_by.name != none)
    assert(cfg.agreed_by.position != none)
    assert(cfg.approved_by.name != none)
    assert(cfg.approved_by.position != none)
    assert(cfg.university_name != none)
    assert(cfg.faculty_name != none)
    assert(cfg.edu_program_name != none)
    assert(cfg.city != none)
    assert(cfg.year != none)

    let un(n) = "_" * n

    let title_page = {
        let top_banner = [
            #set par(spacing: 0.65em)

            #text(weight: "bold", cfg.university_name)

            #cfg.faculty_name

            #cfg.edu_program_name
        ]

        let approve_table = grid(
            columns: (1fr),
            inset: (
                x: 10mm,
                y: 3mm,
            ),
            align: left,

            [
                #set text(weight: "bold")
                Проверил:
            ],

            // [
            //     #set text(weight: "bold")
            //     Руководитель практики от предприятия
            // ],

            // grid(
            //     columns: (3fr, 1fr),
            //     align: left,
            //     [
            //         #cfg.agreed_by.position
            //     ],
            //     [
            //         #cfg.agreed_by.name
            //     ]
            // ),
            // grid(
            //     columns: (1fr, 1fr, 1fr),
            //     align: center,
            //     [
            //         "#un(3)" #un(12) #cfg.year г.
            //     ],
            //     [
            //         #un(13) \ (оценка)
            //     ],
            //     [
            //         #un(13) \ (подпись)
            //     ]
            // ),

            [
                #set text(weight: "bold")

                Руководитель практики от факультета компьютерных наук
            ],

            grid(
                columns: (3fr, 1fr),
                align: left,
                [
                    #cfg.approved_by.position
                ],
                [
                    #cfg.approved_by.name
                ]
            ),
            grid(
                columns: (1fr, 1fr, 1fr),
                align: center,
                [
                    "#un(3)" #un(12) #cfg.year г.
                ],
                [
                    #un(13) \ (оценка)
                ],
                [
                    #un(13) \ (подпись)
                ]
            )
        )

        let center_banner = [
            #set text(size: 14pt, weight: "bold")

            #par(spacing: 0.65em, cfg.project.name)

            по учебной (технологической) практике

            в VK Education Practice
        ]

        let student_info = align(right)[
            #set par(spacing: 1em)

            Выполнил студент

            группы #cfg.student.group

            #cfg.student.name
            
            #set text(size: 12pt)
            #un(13)

            #set text(size: 8pt)
            (подпись)
        ]

        let bottom_banner = [
            #cfg.city -- #cfg.year
        ]

        page(
            header: none,
            footer: none,
            margin: (
                left: 20mm,
                right: 10mm,
                top: 25mm,
                bottom: 15mm,
            )
        )[
            #set align(center)

            #grid(
                columns: (1fr),
                row-gutter: 1fr,
                top_banner,
                center_banner,
                student_info,
                approve_table,
                bottom_banner,
            )
        ]

        counter(page).update(1)
    }

    let outline = {
        pagebreak(weak: true)

        {
            set align(center)
            set text(weight: "bold")

            [СОДЕРЖАНИЕ]
        }

        outline(
            title: none,
            indent: 5mm,
        )
    }

    let outline_and_normal_pages = {
        set page(
            margin: (
                top: 25mm,
                left: 20mm,
                right: 10mm,
                bottom: 47mm,
            ),
            header: [
                #set align(center)
                #set text(weight: "bold")

                #context counter(page).display()
            ],
            footer: []
        )

        set par(
            justify: true,
            leading: 1em,
        )

        set heading(numbering: "1.")

        show heading.where(level: 1): h => {
            set align(center)
            set text(weight: "bold", size: 12pt)

            pagebreak(weak: true)
            if h.numbering != none [
                #counter(heading).display(h.numbering) #h.body
            ] else [
                #h.body
            ]

        }

        show heading.where(level: 2): h => {
            set text(weight: "bold", size: 12pt)

            [#counter(heading).display() #h.body]
        }

        pagebreak(weak: true)
        align(center, text(weight: "bold", size: 12pt, [АННОТАЦИЯ]))
        cfg.annotation

        outline
        body
    }

    set text(
        lang: "ru",
        size: 12pt,
        font: "Times New Roman"
    )

    title_page
    outline_and_normal_pages
}
