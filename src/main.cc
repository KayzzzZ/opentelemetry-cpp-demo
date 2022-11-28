//
// Created by qianlu on 2022/11/24.
//

#include "opentelemetry/sdk/trace/batch_span_processor_factory.h"
#include "opentelemetry/exporters/ostream/span_exporter_factory.h"
#include "opentelemetry/trace/provider.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"

#include "gflags/gflags.h"
//#include "opentelemetry/exporters/otlp/otlp_grpc_exporter_factory.h"
//#include "opentelemetry/exporters/otlp/"
//#include "exporters/otlp/otlp_grpc_exporter/"

DEFINE_string(config_server_addr, "0.0.0.0:8848", "ConfigServer address");

int init_opentelemetry() {
    // initialize grpc exporter
//    opentelemetry::exporter::otlp::OtlpGrpcExporterOptions opts;
//    opts.endpoint = "tracing-analysis-dc-bj.aliyuncs.com:8090";
//    auto exporter = opentelemetry::exporter::otlp::OtlpGrpcExporterFactory::Create(opts);

    auto exporter = opentelemetry::exporter::trace::OStreamSpanExporterFactory::Create();
    // initialize batch span processor
    opentelemetry::sdk::trace::BatchSpanProcessorOptions bsp_opts;
    bsp_opts.schedule_delay_millis = std::chrono::milliseconds(15000); // 15ms
    bsp_opts.max_export_batch_size = 1024;
    bsp_opts.max_queue_size = 4096;
    auto processor =
            opentelemetry::sdk::trace::BatchSpanProcessorFactory::Create(std::move(exporter), bsp_opts);

    // initialize trace provider
    std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
            opentelemetry::sdk::trace::TracerProviderFactory::Create(
                    std::move(processor),
                    opentelemetry::sdk::resource::Resource::Create({
                           {"", ""},
                    }));

    // set global trace provider
    opentelemetry::trace::Provider::SetTracerProvider(provider);
}

opentelemetry::nostd::shared_ptr<opentelemetry::trace::Tracer> get_tracer() {
    return opentelemetry::trace::Provider::GetTracerProvider()->GetTracer("qianlu", "v1.0.0");
}

void initFlags(int argc, char *argv[]) {
    gflags::SetUsageMessage("some usage message");
    gflags::SetVersionString("1.0.0");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    gflags::ShutDownCommandLineFlags();
}

int main(int argc, char *argv[]) {
    initFlags(argc, argv);

    init_opentelemetry();

    for (int i = 0; i < 1000; ++i) {
        if (i / 2) {
            // case 1: has traceId & spanId
            std::array<const uint8_t, 16> tid = {'b', 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
            std::array<const uint8_t, 8> sid = {'a', 1, 2, 3, 4, 5, 6, 7};
            opentelemetry::trace::TraceId traceId(tid);
            opentelemetry::trace::SpanId spanId(sid);
            opentelemetry::trace::StartSpanOptions startSpanOptions;
            startSpanOptions.kind = opentelemetry::trace::SpanKind::kServer;
            // set trace parent
            auto spanContext = opentelemetry::trace::SpanContext(
                    traceId,
                    spanId,
                    opentelemetry::trace::TraceFlags(1),
                    false);
            startSpanOptions.parent = spanContext;
            auto span = get_tracer()->StartSpan("spanName-1-child", startSpanOptions);
            // set attributes
            span->SetAttribute(opentelemetry::nostd::string_view("attributes_key_1"),
                               opentelemetry::common::AttributeValue(opentelemetry::nostd::string_view("attributes_value_1_string")));
            span->End();

        } else {
            // case 2: no traceId & spanId
            opentelemetry::trace::StartSpanOptions startSpanOptions1;
            startSpanOptions1.kind = opentelemetry::trace::SpanKind::kServer;
            auto parentSpan = get_tracer()->StartSpan("spanName-case2-parent", startSpanOptions1);
            auto spanContext = parentSpan->GetContext();

            opentelemetry::trace::StartSpanOptions startSpanOptions2;
            startSpanOptions2.kind = opentelemetry::trace::SpanKind::kClient;
            // set parent span (inherit parent span's traceId & spanId)
            startSpanOptions2.parent = spanContext;
            auto childSpan = get_tracer()->StartSpan("spanName-case2-child", startSpanOptions2);
            // set attributes
            childSpan->SetAttribute(opentelemetry::nostd::string_view("attributes_key_1"),
                               opentelemetry::common::AttributeValue(168));
            childSpan->End();
        }
    }

    return 0;
}